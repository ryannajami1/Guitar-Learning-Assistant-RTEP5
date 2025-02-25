import numpy as np
from scipy.signal import find_peaks
from mingus.core import chords
import matplotlib.pyplot as plt



def read_oscilloscope_csv(file_path):
    # Read the CSV file, skipping the first two rows
    data = np.genfromtxt(file_path, delimiter=',', skip_header=2)
    
    waveform = data[:2000]
    fft = data[2002:]
    # Remove all values at the start of fft where the magnitude in column 2 is nan
    valid_indices = ~np.isnan(fft[:, 1])
    fft = fft[valid_indices]
    return waveform, fft

def determine_peaks(data, threshold, min_height, min_width):

    mask_top = data[0]
    mask_bottom = mask_top - min_height

    highest = data[0]
    highest_index = 0
    lowest = data[0]

    peak_indices = []

    state = 'unknown'

    for i in range(len(data)):
        # move mask
        if data[i] > mask_top:
            mask_top = data[i]
            mask_bottom = mask_top - min_height
            highest = data[i]
            highest_index = i
        elif data[i] < mask_bottom:
            mask_bottom = data[i]
            mask_top = mask_bottom + min_height
            lowest = data[i]
    
        # Check if mask has shifted away from peak or trough
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
    
    return peak_indices


waveform, fft = read_oscilloscope_csv("Oscilloscope Guitar Saves/G chord csv.csv")

time = waveform[:, 0]
voltages = waveform[:, 1]

# Plot the FFT data
fft_freq = fft[:, 0]
fft_magnitude = fft[:, 1]

# Calculate the noise floor of the FFT data
noise_floor = np.mean(fft_magnitude)
peak_threshold = noise_floor*30

# Find peaks in the FFT magnitude data
peaks, _ = find_peaks(fft_magnitude, prominence=peak_threshold)
peaks = determine_peaks(fft_magnitude, peak_threshold, 0.1, 0)

main_frequencies = fft_freq[peaks]
print(main_frequencies)

# Define a dictionary of musical notes and their corresponding frequencies
note_frequencies = {
    'C0': 16.35, 'C#0': 17.32, 'D0': 18.35, 'D#0': 19.45, 'E0': 20.60, 'F0': 21.83, 'F#0': 23.12, 'G0': 24.50, 'G#0': 25.96, 'A0': 27.50, 'A#0': 29.14, 'B0': 30.87,
    'C1': 32.70, 'C#1': 34.65, 'D1': 36.71, 'D#1': 38.89, 'E1': 41.20, 'F1': 43.65, 'F#1': 46.25, 'G1': 49.00, 'G#1': 51.91, 'A1': 55.00, 'A#1': 58.27, 'B1': 61.74,
    'C2': 65.41, 'C#2': 69.30, 'D2': 73.42, 'D#2': 77.78, 'E2': 82.41, 'F2': 87.31, 'F#2': 92.50, 'G2': 98.00, 'G#2': 103.83, 'A2': 110.00, 'A#2': 116.54, 'B2': 123.47,
    'C3': 130.81, 'C#3': 138.59, 'D3': 146.83, 'D#3': 155.56, 'E3': 164.81, 'F3': 174.61, 'F#3': 185.00, 'G3': 196.00, 'G#3': 207.65, 'A3': 220.00, 'A#3': 233.08, 'B3': 246.94,
    'C4': 261.63, 'C#4': 277.18, 'D4': 293.66, 'D#4': 311.13, 'E4': 329.63, 'F4': 349.23, 'F#4': 369.99, 'G4': 392.00, 'G#4': 415.30, 'A4': 440.00, 'A#4': 466.16, 'B4': 493.88,
    'C5': 523.25, 'C#5': 554.37, 'D5': 587.33, 'D#5': 622.25, 'E5': 659.25, 'F5': 698.46, 'F#5': 739.99, 'G5': 783.99, 'G#5': 830.61, 'A5': 880.00, 'A#5': 932.33, 'B5': 987.77,
    'C6': 1046.50, 'C#6': 1108.73, 'D6': 1174.66, 'D#6': 1244.51, 'E6': 1318.51, 'F6': 1396.91, 'F#6': 1479.98, 'G6': 1567.98, 'G#6': 1661.22, 'A6': 1760.00, 'A#6': 1864.66, 'B6': 1975.53,
    'C7': 2093.00, 'C#7': 2217.46, 'D7': 2349.32, 'D#7': 2489.02, 'E7': 2637.02, 'F7': 2793.83, 'F#7': 2959.96, 'G7': 3135.96, 'G#7': 3322.44, 'A7': 3520.00, 'A#7': 3729.31, 'B7': 3951.07,
    'C8': 4186.01
}

def find_closest_note_frequency(frequency):
    closest_note = min(note_frequencies, key=lambda note: abs(note_frequencies[note] - frequency))
    return closest_note, note_frequencies[closest_note]

# Find and print the closest notes for the main frequencies
notes = []
for freq in main_frequencies:
    note, note_freq = find_closest_note_frequency(freq)
    notes.append(note)
    print(f"Frequency: {freq:.2f} Hz is closest to Note: {note} ({note_freq:.2f} Hz)")

# Remove numbers from the notes and remove duplicates
notes = list(set(note.rstrip('0123456789') for note in notes))
print(notes)
notes.sort()
print(notes)

# Find the resulting chord
chord = chords.determine(notes, shorthand=True)
print(chord)

# Plot the waveform data
plt.figure(figsize=(10, 6))

# Calculate the RMS of the voltage
rms_voltage = np.sqrt(np.mean(voltages**2))

plt.subplot(2, 1, 1)
plt.plot(time, voltages)
plt.axhline(y=rms_voltage, color='r', linestyle='--', label='RMS Voltage')
plt.xlabel('Time (s)')
plt.ylabel('Voltage (V)')
plt.title('Oscilloscope Data - Waveform')
plt.legend()
plt.grid(True)

plt.subplot(2, 1, 2)
plt.plot(fft_freq, fft_magnitude, label='FFT')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.title('Oscilloscope Data - FFT')
plt.legend()
plt.grid(True)
# Plot the noise floor and threshold
plt.axhline(y=noise_floor, color='r', linestyle='--', label='Noise Floor')
plt.axhline(y=peak_threshold, color='orange', linestyle='--', label='Threshold')

# Highlight significant frequencies as dots
plt.plot(peaks, fft_magnitude[peaks], "x")

# Add legend for the new lines
plt.legend()

plt.tight_layout()
plt.show()