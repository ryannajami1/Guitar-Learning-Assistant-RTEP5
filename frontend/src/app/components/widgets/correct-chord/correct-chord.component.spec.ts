import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CorrectChordComponent } from './correct-chord.component';

describe('CorrectChordComponent', () => {
  let component: CorrectChordComponent;
  let fixture: ComponentFixture<CorrectChordComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [CorrectChordComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(CorrectChordComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
