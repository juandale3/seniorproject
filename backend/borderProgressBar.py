import customtkinter as ctk
import tkinter as tk
import math

class SpinningLoaderApp(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("Spinning Loader")
        self.geometry("300x300")  # Increased window size for a bigger loader

        # Increased canvas size to accommodate the larger loader
        self.canvas = tk.Canvas(self, width=200, height=200, bg='white', highlightthickness=0)
        self.canvas.pack(padx=50, pady=50)

        # Parameters for the loader
        self.loader_size = 100  # Increased diameter of the loader for a bigger circle
        self.num_segments = 8  # Number of segments in the loader
        self.active_segment = 0  # The currently "active" or highlighted segment

        # Draw the initial loader segments
        self.segments = self.draw_loader()

        # Start the spinning animation
        self.animate_loader()

    def draw_loader(self):
        segments = []
        center_x, center_y = 100, 100  # Adjusted center of the canvas/loader for the new size
        radius = self.loader_size / 2  # Radius of the loader

        for i in range(self.num_segments):
            angle = 360 / self.num_segments * i
            end_angle = angle + (360 / self.num_segments)
            coords = self.arc_coords(center_x, center_y, radius, angle, end_angle)
            segment = self.canvas.create_arc(coords, start=angle, extent=(360 / self.num_segments - 2), style=tk.ARC, outline='light grey', width=2)
            segments.append(segment)
        
        return segments

    def arc_coords(self, cx, cy, r, start_angle, end_angle):
        # Calculate the bounding box for the arc
        x0 = cx - r
        y0 = cy - r
        x1 = cx + r
        y1 = cy + r
        return [x0, y0, x1, y1]

    def animate_loader(self):
        # Update the color of each segment
        for i in range(self.num_segments):
            color = 'cyan' if i == self.active_segment else 'black'
            self.canvas.itemconfig(self.segments[i], outline=color)
        
        # Move to the next segment
        self.active_segment = (self.active_segment + 1) % self.num_segments

        # Schedule the next frame
        self.canvas.after(100, self.animate_loader)

if __name__ == "__main__":
    app = SpinningLoaderApp()
    app.mainloop()
