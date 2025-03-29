import {Injectable} from '@angular/core';
import {EChordStatus} from '../models/chord-status.enum';

@Injectable({
  providedIn: 'root'
})
export class ChordsService {

  constructor() {
  }

  lastChords: EChordStatus[] = [];

  showCorrectChordModal: boolean = false;
  showWrongChordModal: boolean = false;
  showTimeoutModal: boolean = false;

  getAccuracy(): number {

    if (!this.lastChords.length) return 0;

    let accurateCount = 0;
    for (const chordStatus of this.lastChords) {
      if (chordStatus === EChordStatus.Correct) {
        accurateCount++;
      }
    }
    return accurateCount / this.lastChords.length;
  }

  getLast5Chords(): EChordStatus[] {
    if (this.lastChords.length >= 5) {
      return this.lastChords.slice(-5);
    } else {
      return this.lastChords;
    }
  }

  timeoutReached(): void {
    this.showTimeoutModal = true;
    this.lastChords.push(EChordStatus.Timeout)
    this.startResetModalTimer();
    console.log(this.lastChords)
  }

  correctChordPlayed(): void {
    this.showCorrectChordModal = true;
    this.lastChords.push(EChordStatus.Correct)
    this.startResetModalTimer();
    console.log(this.lastChords)
  }

  wrongChordPlayed(): void {
    this.showWrongChordModal = true;
    this.lastChords.push(EChordStatus.Wrong)
    this.startResetModalTimer();
    console.log(this.lastChords)
  }

  startResetModalTimer(): void {
    setTimeout(() => {
      this.showCorrectChordModal = false;
      this.showTimeoutModal = false;
      this.showWrongChordModal = false;
    }, 1000); // 1000 milliseconds = 1 second
  }

}
