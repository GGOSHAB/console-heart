#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <numeric>
#include <ranges>

#include "includes/math.hpp"
#include "includes/raymarching.hpp"

#ifdef _WIN32
#include <windows.h>
void get_console_size(int &width, int &height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
void get_console_size(int &width, int &height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
}
#endif

const float ASPECT_RATIO = 0.5f;
const int ITERS = 150;
const double eps = 1e-12;

int WIDTH, HEIGHT;
double ASPECT;
std::string frame_buffer;

const std::string light = " .:!l4$GW@";

inline void run() {
    auto time_start = std::chrono::steady_clock::now(); 
    auto time_now = std::chrono::steady_clock::now();
    
    int threads_num = std::thread::hardware_concurrency();
    if (threads_num == 0) threads_num = 4;
    std::vector<std::thread> threads(threads_num);
    const int ROW_PER_THREAD = (HEIGHT + threads_num - 1) / threads_num;

    math::vec3d ro = {0, 0, 0};
    sphere sphl({-1.0, 0.0, 1.5}, 0.5), sphr({1.0, 0.0, 1.5}, 0.5);
    std::vector<rm_object*> objects {&sphl, &sphr};
    auto min_dist = [&](const math::vec3d &p) {
        // double res = std::numeric_limits<double>::infinity();
        // for (rm_object *obj : objects) {
        //     auto cur_dist = obj->dist(p);
        //     if (res > cur_dist) {
        //         res = cur_dist;
        //     }
        // }
        // return res;

        double res = objects[0]->dist(p);
        const double k = 0.67;
        for (int i = 1; i < objects.size(); ++i) {
            res = math::smooth_min(res, objects[i]->dist(p), k);
        }
        return res;
    };
    auto get_norm = [&](const math::vec3d &p) {
        const double eps = 1e-6;
        return math::norm(math::vec3d{
            min_dist({p[0] + eps, p[1], p[2]}) - min_dist({p[0] - eps, p[1], p[2]}),
            min_dist({p[0], p[1] + eps, p[2]}) - min_dist({p[0], p[1] - eps, p[2]}),
            min_dist({p[0], p[1], p[2] + eps}) - min_dist({p[0], p[1], p[2] - eps}),
        });
    };

    math::vec3d light_dir = math::vec3d{-1.0, -1.5, 1.5}.norm();

    double t = 0.0;

    auto render_rows = [&](int sr, int er) {
        if (er > HEIGHT) er = HEIGHT;
        for (int i = sr; i < er; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                math::vec2d uv = math::vec2d{(double)j / WIDTH * 2.0 - 1.0, 1.0 - (double)i / HEIGHT * 2.0};
                uv[0] *= ASPECT;
                math::vec3d rd = {uv[0], uv[1], 1.0};
                rd.norm();
                
                math::vec3d ray = ro;
                for (int it = 0; it < ITERS; ++it) {
                    double dist = min_dist(ray);
                    if (dist < eps) break;
                    ray += rd * dist;
                }

                int color = 0;
                double dist = min_dist(ray);
                if (dist < eps) {
                    math::vec3d n = get_norm(ray);
                    double prod = math::clamp(n * (-1.0 * light_dir), 0.0, 1.0);
                    // double prod = 1.0;
                    color = std::max<int>((light.size() - 1) * prod, 1);
                }

                frame_buffer[i * WIDTH + j] = light[color];
            }
        }
    };

    while (true) {
        time_now = std::chrono::steady_clock::now();
        t = std::chrono::duration<double>(time_now - time_start).count();

        light_dir = math::vec3d{-std::cos(t), -1.0, -std::sin(t)}.norm();
        sphl.c[0] = 1.5 * std::cos(t * 0.5);
        sphr.c[0] = -1.5 * std::cos(t * 0.5);
        

        for (int i = 0; i < threads_num; ++i) {
            threads[i] = std::thread(render_rows, i * ROW_PER_THREAD, (i + 1) * ROW_PER_THREAD);
        }

        for (int i = 0; i < threads_num; ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }

        std::cout << "\x1b[H";
        std::cout.write(frame_buffer.data(), frame_buffer.size());
        std::cout.flush();
    }
}

int main() {
    WIDTH = 80, HEIGHT = 25;
    get_console_size(WIDTH, HEIGHT);
    ASPECT = WIDTH / (double)HEIGHT * ASPECT_RATIO;
    frame_buffer.resize(WIDTH * HEIGHT);
    
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::setlocale(LC_ALL, "en_US.UTF-8");
    std::cout << "\x1b[2J" << std::flush;
    
    run();
    
    return 0;
}