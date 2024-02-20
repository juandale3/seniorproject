import tkinter as tk

class BorderProgressBar(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Border Progress Bar")
        self.geometry("600x400")

        # Top progress bar
        self.top_progress = tk.Canvas(self, height=5, bg='lightgray')
        self.top_progress.pack(fill='x')

        # Left progress bar
        self.left_progress = tk.Canvas(self, width=5, bg='lightgray')
        self.left_progress.pack(side='left', fill='y')

        # Right progress bar
        self.right_progress = tk.Canvas(self, width=5, bg='lightgray')
        self.right_progress.pack(side='right', fill='y')

        # Bottom progress bar
        self.bottom_progress = tk.Canvas(self, height=5, bg='lightgray')
        self.bottom_progress.pack(side='bottom', fill='x')

        # Start the progress animation
        self.animate_progress()

    def animate_progress(self, step=10):
        # Reset the progress if it exceeds the window size
        if step >= self.winfo_width() or step >= self.winfo_height():
            step = 40

        # Clear previous progress
        self.top_progress.delete('progress')
        self.left_progress.delete('progress')
        self.right_progress.delete('progress')
        self.bottom_progress.delete('progress')

        # Update progress
        self.top_progress.create_rectangle(0, 0, step, 5, fill='blue', tags='progress')
        self.left_progress.create_rectangle(0, 0, 5, step, fill='blue', tags='progress')
        self.right_progress.create_rectangle(0, self.winfo_height()-step, 5, self.winfo_height(), fill='blue', tags='progress')
        self.bottom_progress.create_rectangle(self.winfo_width()-step, 0, self.winfo_width(), 5, fill='blue', tags='progress')

        # Increase the step for the next frame
        step += 0.1

        # Schedule next animation frame
        self.after(100, lambda: self.animate_progress(step))

if __name__ == "__main__":
    app = BorderProgressBar()
    app.mainloop()
