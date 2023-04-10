import foo

foo.foo()

print(foo.py_int(123))
print(foo.py_bool(9999))
print(foo.py_str("test abc."))
print(foo.py_float(3))
print(foo.py_list(5))


# Noneable
print("----")
print(foo.py_foo(None))
print(foo.py_foo(123))
print(foo.py_foo(False))
print(foo.py_foo("test abc."))
print(foo.py_foo(12.3))
print(foo.py_foo([]))
print(foo.py_foo([1, "d"]))
print(foo.py_foo({}))
print(foo.py_foo({10: 100, 'abc': 'def', 21.2: 33.3, True: False}))

# Callback 関数
def callback(value) :
    print("call xxx. " + str(value))
    return "xxx"
print(foo.py_apply(callback));