from threading import Thread

def getInput():
	while True:
		print(input("your name: "))

def sameTime():
	while True:
		print("Hello there")

t1 = Thread(target=getInput)
t2 = Thread(target=sameTime)

t1.start()
t2.start()

t1.join()
t2.join()

print("This always happens last")