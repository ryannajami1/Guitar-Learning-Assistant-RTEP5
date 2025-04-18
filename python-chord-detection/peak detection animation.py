import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

def read_oscilloscope_csv(file_path):
    # Read the CSV file, skipping the first two rows
    data = np.genfromtxt(file_path, delimiter=',', skip_header=2)
    
    waveform = data[:2000]
    fft = data[2002:]
    # Remove all values at the start of fft where the magnitude in column 2 is nan
    valid_indices = ~np.isnan(fft[:, 1])
    fft = fft[valid_indices]
    return waveform, fft

def animate_peak_detection(data, fft_freq, threshold, min_height):
    # Setup the figure
    fig, ax = plt.subplots()
    ax.set_xlim(fft_freq[0], fft_freq[-1])
    ax.set_ylim(0, 1.1 * max(data))
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Magnitude")
    ax.set_title("Peak Detection on FFT Data from G Chord")
    ax.grid(True)

    # Initialize the plot elements
    line, = ax.plot(fft_freq, data, label="FFT", color="blue")
    mask_bar = ax.fill_between(fft_freq, 0, 0, color="green", alpha=0.3, label="Mask Range")
    peak_dots, = ax.plot([], [], "x", label="Detected Peaks", color="red")
    vertical_line = ax.axvline(x=fft_freq[0], color="orange", linestyle="--", label="Current Index")
    thresh_line = ax.axhline(y=threshold, color="red", linestyle="--", label="Threshold")

    # Annotation for frame progress
    progress_text = ax.text(0.95, 0.95, "", transform=ax.transAxes, fontsize=10, ha="right", va="top")

    # Variables to store peak detection state
    peak_indices = []
    mask_top = data[0]
    mask_bottom = mask_top - min_height
    highest = data[0]
    highest_index = 0
    lowest = data[0]
    state = 'unknown'

    delay_i = 0

    plt.show(block=False)
    def update(frame):
        nonlocal delay_i, mask_top, mask_bottom, mask_bar, highest, highest_index, lowest, state, peak_indices

        # Delay logic
        # if delay_i < 100:
        #     delay_i += 1
        #     return mask_bar, peak_dots, vertical_line, progress_text
        
        # Update the mask and detection logic
        i = frame
        if i >= len(data):  # Stop if we exceed the data length
            return mask_bar, peak_dots, vertical_line, progress_text

        # Move mask
        if data[i] > mask_top:
            mask_top = data[i]
            mask_bottom = mask_top - min_height
            highest = data[i]
            highest_index = i
        elif data[i] < mask_bottom:
            mask_bottom = data[i]
            mask_top = mask_bottom + min_height
            lowest = data[i]

        # Detect peaks/troughs
        if highest > mask_top:
            if state == 'unknown' or state == 'find peak':
                state = 'find trough'
                if highest > threshold:
                    peak_indices.append(highest_index)
                lowest = mask_bottom

        if lowest < mask_bottom:
            if state == 'unknown' or state == 'find trough':
                state = 'find peak'
                highest = mask_top
                highest_index = i

        # Update the mask bar
        mask_bar.remove()  # Remove the previous mask bar
        mask_bar = ax.fill_between(fft_freq[:i+1], mask_bottom, mask_top, color="green", alpha=0.3)

        # Update the vertical line
        vertical_line.set_xdata(fft_freq[i])

        # Update the detected peaks
        peak_dots.set_data(fft_freq[peak_indices], data[peak_indices])

        # Update progress text
        progress_text.set_text(f"Current state: {state}")

        return mask_bar, peak_dots, vertical_line, progress_text

    # Initialize animation
    ani = FuncAnimation(fig, update, frames=len(data), interval=10, blit=False)

    # Add legend and show
    ax.legend()
    plt.show()

## Sin wave ====================================
# fft_data = np.sin(2 * np.pi * np.linspace(0, 5, 500)) + 1 # Example FFT magnitudes as a sine wave
# fft_frequencies = np.linspace(0, 1000, len(fft_data))  # Example FFT frequencies
# noise_floor = np.mean(fft_data)
# threshold = noise_floor * 1
# min_height = 0.8
# animate_peak_detection(fft_data, fft_frequencies, threshold, min_height)

# FFT data
waveform, fft = read_oscilloscope_csv("Oscilloscope Guitar Saves/scope_4.csv")

time = waveform[:, 0]
voltages = waveform[:, 1]

# Plot the FFT data
fft_freq = fft[:, 0]
fft_magnitude = fft[:, 1]

# Calculate the noise floor of the FFT data
noise_floor = np.mean(fft_magnitude)
peak_threshold = noise_floor*30

# Find peaks in the FFT magnitude data
# peaks, _ = find_peaks(fft_magnitude, prominence=peak_threshold)
animate_peak_detection(fft_magnitude, fft_freq, peak_threshold, 0.1)