.PHONY: all 
all: simple_recording_test simple_playback_test

.PHONY: clean
clean:
	rm simple_playback_test simple_recording_test

simple_playback_test: playback_test.cpp
	$(CXX) $^ -lasound -o $@

simple_recording_test: recording_test.cpp
	$(CXX) $^ -lasound -o $@
