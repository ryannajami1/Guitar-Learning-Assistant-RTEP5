import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MobilePracticeComponent } from './mobile-practice.component';

describe('MobilePracticeComponent', () => {
  let component: MobilePracticeComponent;
  let fixture: ComponentFixture<MobilePracticeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [MobilePracticeComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(MobilePracticeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
