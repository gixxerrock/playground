from tkinter import *

root = Tk()  # create a root widget
root.title("Tk Example")
root.configure(background="yellow")
root.minsize(200, 200)  # width, height
root.maxsize(500, 500)
root.geometry("300x300+50+50")  # width x height + x + y

# place a label on the root window
message = Label(root, text="Hello, World!")
message.pack()

root.mainloop()
