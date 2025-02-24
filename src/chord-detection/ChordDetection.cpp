#include <cstdio>
#include <vector>

using namespace std;

vector<int> detect_peaks(const vector<float> &data, float threshold, float min_peak_width, float min_peak_height) {
    
    // Find peaks and valleys then filter based on distance
    
    vector<int> peak_indexes;

    float current_value;
    float prev_value;

    float prev_highest = data[0];
    int prev_highest_index = 0;

    float prev_lowest = data[0];
    int prev_lowest_index = 0;

    bool ascending = false;

    for (size_t i = 1; i < data.size() - 1; i++) {
        current_value = data[i];

        // Push the boundaries

        if (current_value > prev_highest) {
            prev_highest = current_value;
            prev_highest_index = i;
        }
        else if (current_value < prev_lowest) {
            prev_lowest = current_value;
        }

        // Check for peaks and troughs

        if ((current_value - prev_lowest) > min_peak_height) {
            // Prev lowest was a valid trough. We are now ascending

            if (!ascending) {
                ascending = true;

                // Reset the highest value to find new peak
                prev_highest = current_value;
                prev_highest_index = i;
            }

            // printf("i: %zu\nmax: %f\ncur: %f\n", i, prev_highest, current_value);
        }

        if ((prev_highest - current_value) > min_peak_height) {
            // Prev highest was a valid peak. We are now descending
            if (ascending) {
                ascending = false;

                if (prev_highest > threshold) {
                    peak_indexes.push_back(prev_highest_index);
                }

                // Reset lowest value to find new trough
                prev_lowest = current_value;
                prev_lowest_index = i;
            }
            // printf("i: %zu\nmin: %f\ncur: %f\n", i, prev_lowest, current_value);

        }
    }

    return peak_indexes;
}


int main() {

    // vector<float> data = {0.0, 1, 2, 3, 4, 3, 2, 1, 0, 1, 2, 3, 4, 3, 2, 1, 0};
    vector<float> data = {0,1,2,3,4,5,4,3,2,7,8,9,6,5,4,8,7,6,5,4,5,6,7,0,1};

    vector<int> indexes = detect_peaks(data, 6, 2, 2);

    for (int index : indexes) {
        printf("%d ", index);
    }
    printf("\n");

}