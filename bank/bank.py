greet = input ('Greeting: ')

if 'hello' in greet.lower():
    print('$0')
elif 'h' in greet.lower()[0]:
    print('$20')
else:
    print('$100')
