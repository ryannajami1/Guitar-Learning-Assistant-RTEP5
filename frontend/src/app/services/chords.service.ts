import {Injectable} from '@angular/core';
import {EChordStatus} from '../models/chord-status.enum';

@Injectable({
  providedIn: 'root'
})
export class ChordsService {

  constructor() {
  }

  currentChordToBePlayed: string = '';

  chordGenerationIncludeA: boolean = true;
  chordGenerationIncludeB: boolean = true;
  chordGenerationIncludeC: boolean = true;
  chordGenerationIncludeD: boolean = true;
  chordGenerationIncludeE: boolean = true;
  chordGenerationIncludeF: boolean = true;
  chordGenerationIncludeG: boolean = true;

  chordGenerationIncludeStandard: boolean = true;
  chordGenerationIncludeMaj: boolean = true;
  chordGenerationIncludeMin: boolean = true;
  chordGenerationInclude7: boolean = true;
  chordGenerationIncludeMaj7: boolean = true;
  chordGenerationIncludeMin7: boolean = true;
  chordGenerationIncludeSus4: boolean = true;
  chordGenerationIncludeDim: boolean = true;
  chordGenerationIncludeAug: boolean = true;

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

  chordEventHandler(status: EChordStatus): void {
    if (status === EChordStatus.Timeout) {
      this.showTimeoutModal = true;
      this.lastChords.push(EChordStatus.Timeout)
    } else if (status === EChordStatus.Correct) {
      this.showCorrectChordModal = true;
      this.lastChords.push(EChordStatus.Correct)
    } else if (status === EChordStatus.Wrong) {
      this.showWrongChordModal = true;
      this.lastChords.push(EChordStatus.Wrong)
    }
    setTimeout(() => {
      this.showCorrectChordModal = false;
      this.showTimeoutModal = false;
      this.showWrongChordModal = false;
      this.currentChordToBePlayed = this.getRandomGuitarChord();
    }, 1000); // 1000 milliseconds = 1 second
  }

  getRandomGuitarChord(): string {
    let notes = [];

    if (this.chordGenerationIncludeA) {
      notes.push('A');
    }
    if (this.chordGenerationIncludeB) {
      notes.push('B');
    }
    if (this.chordGenerationIncludeC) {
      notes.push('C');
    }
    if (this.chordGenerationIncludeD) {
      notes.push('D');
    }
    if (this.chordGenerationIncludeE) {
      notes.push('E');
    }
    if (this.chordGenerationIncludeF) {
      notes.push('F');
    }
    if (this.chordGenerationIncludeG) {
      notes.push('G');
    }

    let chordTypes = [];

    if (this.chordGenerationIncludeStandard) {
      chordTypes.push('');
    }
    if (this.chordGenerationIncludeMaj) {
      chordTypes.push('maj');
    }
    if (this.chordGenerationIncludeMin) {
      chordTypes.push('min');
    }
    if (this.chordGenerationInclude7) {
      chordTypes.push('7');
    }
    if (this.chordGenerationIncludeMaj7) {
      chordTypes.push('maj7');
    }
    if (this.chordGenerationIncludeMin7) {
      chordTypes.push('min7');
    }
    if (this.chordGenerationIncludeSus4) {
      chordTypes.push('sus4');
    }
    if (this.chordGenerationIncludeDim) {
      chordTypes.push('dim');
    }
    if (this.chordGenerationIncludeAug) {
      chordTypes.push('aug');
    }


    const randomNote = notes[Math.floor(Math.random() * notes.length)];
    const randomChordType = chordTypes[Math.floor(Math.random() * chordTypes.length)];

    return `${randomNote} ${randomChordType}`;
  }
}
