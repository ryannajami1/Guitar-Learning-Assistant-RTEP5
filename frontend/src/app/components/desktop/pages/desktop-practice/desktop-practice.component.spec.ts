import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DesktopPracticeComponent } from './desktop-practice.component';

describe('DesktopPracticeComponent', () => {
  let component: DesktopPracticeComponent;
  let fixture: ComponentFixture<DesktopPracticeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DesktopPracticeComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DesktopPracticeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
