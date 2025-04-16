import numpy as np

filename = 'scope_4'

data = np.genfromtxt('Oscilloscope Guitar saves/' + filename + '.csv', delimiter=',', skip_header=2)

waveform = data[:2000]
fft = data[2002:]
# Remove all values at the start of fft where the magnitude in column 2 is nan
valid_indices = ~np.isnan(fft[:, 1])
fft = fft[valid_indices]

# Save waveform with headers
waveform_header = 'time,voltage'
np.savetxt('Oscilloscope Guitar saves/' + filename + '_waveform.csv', waveform, delimiter=',', header=waveform_header, comments='')

# Save fft with headers
fft_header = 'frequency,magnitude'
np.savetxt('Oscilloscope Guitar saves/' + filename + '_fft.csv', fft, delimiter=',', header=fft_header, comments='')