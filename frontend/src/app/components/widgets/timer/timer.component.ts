import {Component, ElementRef, EventEmitter, Input, Output, Renderer2, ViewChild} from '@angular/core';
import {ProgressBar} from 'primeng/progressbar';

@Component({
  selector: 'app-timer',
  standalone: true,
  imports: [ProgressBar],
  templateUrl: './timer.component.html',
  styleUrl: './timer.component.scss'
})
export class TimerComponent {
  @Input() currentChord: string = '';
  @Output() timeOverEvent = new EventEmitter<null>();
  @ViewChild("loadingBarActive") loadingBarActive: ElementRef | undefined;

  startValue: number = 5;
  currentValue: number = 0;
  interval: any;

  constructor(private renderer: Renderer2) {
  }

  ngAfterViewInit(): void {
    this.startCountdown();
  }

  ngOnChanges(): void {
    this.restart();
  }

  startCountdown(): void {
    this.startLoading();
    this.countdown();
  }

  startLoading(): void {
    this.currentValue = this.startValue;
    this.renderer.setStyle(this.loadingBarActive?.nativeElement, 'transition', `width ${this.currentValue}s linear`);
    this.renderer.addClass(this.loadingBarActive?.nativeElement, 'shrink');
  }

  countdown(): void {
    this.interval = setInterval(() => {
      if (this.currentValue <= 0) {
        this.currentValue = 0;
        clearInterval(this.interval);
        this.timeOverEvent.next(null);
      } else {
        this.currentValue--;
      }
    }, 1000);
  }

  restart() {
    clearInterval(this.interval);
    this.renderer.setStyle(this.loadingBarActive?.nativeElement, 'transition', '0s');
    this.renderer.removeClass(this.loadingBarActive?.nativeElement, 'shrink');
    setTimeout(() => {
      this.startCountdown();
    }, 10); // Small delay to ensure the transition is re-enabled
  }

}
