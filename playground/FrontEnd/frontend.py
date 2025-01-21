import tkinter as tk

root = tk.Tk()  # create a root widget
root.title("Audio Playground Front End")
root.configure(background="yellow")
root.minsize(200, 200)  # width, height
root.maxsize(500, 500)
root.geometry("300x300+50+50")  # width x height + x + y

# place a label on the root window
message = tk.Label(root, text="Hello, World!", fg="white", bg="black", width=10, height=10)
entry = tk.Entry()

message.pack()
entry.pack()

frame_a = tk.Frame()
frame_b = tk.Frame()

label_a = tk.Label(master=frame_a, text="I'm in Frame A")
label_a.pack()

label_b = tk.Label(master=frame_b, text="I'm in Frame B")
label_b.pack()

frame_a.pack()
frame_b.pack()

root.mainloop()
