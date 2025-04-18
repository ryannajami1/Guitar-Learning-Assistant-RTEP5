import {Injectable} from '@angular/core';
import {EChordStatus} from '../models/chord-status.enum';
import {BehaviorSubject, Observable} from 'rxjs';
import {ChordGenerationSettings, defaultChordGenerationSettings} from '../models/chord-generation-settings.model';
import {WsService} from './ws.service';

@Injectable({
  providedIn: 'root'
})
export class ChordsService {


  constructor(private wsService: WsService) {
    let chordGenerationSettings = sessionStorage.getItem('chordGenerationSettings');
    if (chordGenerationSettings) {
      this.chordGenerationSettings = JSON.parse(chordGenerationSettings);
    }

    this.wsService.getMessages().subscribe((event) => {
      this.handleWsMessage(event);
    })
  }

  timerStartValueMs: number = 5000; // in ms
  timerCurrentValueMs: number = 0; // in ms
  timerInterval: any;


  private _timerRunning = new BehaviorSubject<boolean>(false);
  timerRunning$: Observable<boolean> = this._timerRunning.asObservable();

  currentChordToBePlayed: string = '';

  chordGenerationSettings: ChordGenerationSettings = defaultChordGenerationSettings;

  lastChords: EChordStatus[] = [];
  lastResponseTimes: number [] = [];

  showCorrectChordModal: boolean = false;
  showWrongChordModal: boolean = false;
  showTimeoutModal: boolean = false;

  wsChordReceived: boolean = false;
  wsLastChordReceived: string = '';

  handleWsMessage(message: any): void {
    this.wsChordReceived = true;
    this.wsLastChordReceived = message;
    if (this.wsLastChordReceived == this.currentChordToBePlayed) {
      this.chordEventHandler(EChordStatus.Correct)
    }
  }

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

  getResponseTimeAvg(): number {
    if (!this.lastResponseTimes.length) return 0;

    let sum = 0;
    for (const responseTime of this.lastResponseTimes) {
      sum += responseTime;
    }
    return sum / this.lastResponseTimes.length;
  }

  getNewCurrentChord(): void {
    this.currentChordToBePlayed = this.getRandomGuitarChord();
    this.timerRestart();
  }

  chordEventHandler(status: EChordStatus): void {
    if (this.showTimeoutModal || this.showCorrectChordModal || this.showWrongChordModal) {
      return;
    }


    this.timerStop();
    this.lastResponseTimes.push(this.timerStartValueMs - this.timerCurrentValueMs);

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
      this.getNewCurrentChord();
    }, 1000); // 1000 milliseconds = 1 second
  }

  timerStart(): void {
    this.wsChordReceived = false;
    this.wsLastChordReceived = '';
    this._timerRunning.next(true);
    this.timerCurrentValueMs = this.timerStartValueMs;

    this.timerInterval = setInterval(() => {
      if (this.timerCurrentValueMs <= 0) {
        this.timerCurrentValueMs = 0;
        clearInterval(this.timerInterval);
        this.wsChordReceived ? this.chordEventHandler(EChordStatus.Wrong) : this.chordEventHandler(EChordStatus.Timeout);
      } else {
        this.timerCurrentValueMs = this.timerCurrentValueMs - 50;
      }
    }, 50);
  }

  timerStop(): void {
    this._timerRunning.next(false);
    clearInterval(this.timerInterval);
  }

  timerRestart(): void {
    this._timerRunning.next(false);
    clearInterval(this.timerInterval);
    setTimeout(() => {
      this.timerStart();
    }, 10); // Small delay to ensure the transition is re-enabled
  }


  getRandomGuitarChord(): string {
    let notes = [];

    if (this.chordGenerationSettings.notes.A) {
      notes.push('A');
    }
    if (this.chordGenerationSettings.notes.B) {
      notes.push('B');
    }
    if (this.chordGenerationSettings.notes.C) {
      notes.push('C');
    }
    if (this.chordGenerationSettings.notes.D) {
      notes.push('D');
    }
    if (this.chordGenerationSettings.notes.E) {
      notes.push('E');
    }
    if (this.chordGenerationSettings.notes.F) {
      notes.push('F');
    }
    if (this.chordGenerationSettings.notes.G) {
      notes.push('G');
    }

    let chordTypes = [];

    if (this.chordGenerationSettings.types.standard) {
      chordTypes.push('');
    }
    if (this.chordGenerationSettings.types.maj) {
      chordTypes.push('maj');
    }
    if (this.chordGenerationSettings.types.min) {
      chordTypes.push('min');
    }
    if (this.chordGenerationSettings.types.seven) {
      chordTypes.push('7');
    }
    if (this.chordGenerationSettings.types.maj7) {
      chordTypes.push('maj7');
    }
    if (this.chordGenerationSettings.types.min7) {
      chordTypes.push('min7');
    }
    if (this.chordGenerationSettings.types.sus4) {
      chordTypes.push('sus4');
    }
    if (this.chordGenerationSettings.types.dim) {
      chordTypes.push('dim');
    }
    if (this.chordGenerationSettings.types.aug) {
      chordTypes.push('aug');
    }


    const randomNote = notes[Math.floor(Math.random() * notes.length)];
    const randomChordType = chordTypes[Math.floor(Math.random() * chordTypes.length)];

    return `${randomNote} ${randomChordType}`;
  }
}
