import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AccuracyComponent } from './accuracy.component';

describe('AccuracyComponent', () => {
  let component: AccuracyComponent;
  let fixture: ComponentFixture<AccuracyComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [AccuracyComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(AccuracyComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
