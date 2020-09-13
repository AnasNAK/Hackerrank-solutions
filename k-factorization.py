def getResult(n, a):
    
    if n is 1:
        return []
    
    for x in a:
        if n % x is 0:
            break
    else:
        return False
    
    for x in a:
        if n % x is 0:
            result = getResult(int(n/x), a)
            # print("result is ", result, "for", n/x, "with", a)
            if result is not False:
                result.append(x)
                return result

    return False

n, k = map(int, input().split())
a = list(map(int, input().split()))
a.sort(reverse=True)

result = getResult(n, a)
if result is False:
    print(-1)
else:
    current = 1
    print(current, end=' ')
    for x in result:
        current *= x
        print(current, end=' ')
