import string

a = string.ascii_letters + string.digits + string.punctuation + ' '

inp = open('my_ans.txt', 'r')

def compare(a, b) -> bool:
    print(f'{a} vs {b}: <=>')
    tmp = inp.readline()
    ans = inp.readline()
    ans = ans.strip()
    if ans == '<':
        return True
    elif ans == '>' or ans == '=':
        return False
    else:
        print(f'Wrong input: expected <, > or =, found {ans}')
        return compare(a, b)
    

b = []
for i in range(len(a)):
    l, r = 0, i
    while r - l > 1:
        m = l + (r - l) // 2
        if compare(a[i], b[m]):
            l = m
        else:
            r = m
    b.insert(l, a[i])

print(b)
out = open('output.txt', mode='w')
for ch in b:
    out.write(ch)