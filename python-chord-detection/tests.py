from math import log2

def note_lookup(freq):
    notes = ["A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"]
    note_number = round(12 * log2(freq / 440.0))
    print(note_number, note_number % 12, notes[note_number % 12])
    return note_number

note_lookup(440)
note_lookup(300)
note_lookup(27.5)
note_lookup(4186.01)