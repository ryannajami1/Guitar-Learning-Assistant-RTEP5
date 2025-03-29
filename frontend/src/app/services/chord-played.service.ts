import { Injectable } from '@angular/core';


export enum EChordStatus {
  Correct = "correct",
  Wrong = "wrong",
  Timeout = "timeout",
  None = "none"
}

@Injectable({
  providedIn: 'root'
})
export class ChordPlayedService {

  constructor() { }

  last5chords: EChordStatus[] = [EChordStatus.None, EChordStatus.None, EChordStatus.None, EChordStatus.None, EChordStatus.None];

  showCorrectChordModal: boolean = false;
  showWrongChordModal: boolean = false;
  showTimeoutModal: boolean = false;


  timeoutReached(): void {
    this.showTimeoutModal = true;
    this.last5chords.shift()
    this.last5chords.push(EChordStatus.Timeout)
    this.startResetModalTimer();
  }

  correctChordPlayed(): void {
    this.showCorrectChordModal = true;
    this.last5chords.shift()
    this.last5chords.push(EChordStatus.Correct)
    this.startResetModalTimer();
  }

  wrongChordPlayed(): void {
    this.showWrongChordModal = true;
    this.last5chords.shift()
    this.last5chords.push(EChordStatus.Wrong)
    this.startResetModalTimer();
  }

  startResetModalTimer(): void {
    setTimeout(() => {
      this.showCorrectChordModal = false;
      this.showTimeoutModal = false;
      this.showWrongChordModal = false;
    }, 1000); // 1000 milliseconds = 1 second
  }

}
