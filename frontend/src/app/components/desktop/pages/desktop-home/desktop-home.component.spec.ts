import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DesktopHomeComponent } from './desktop-home.component';

describe('DesktopHomeComponent', () => {
  let component: DesktopHomeComponent;
  let fixture: ComponentFixture<DesktopHomeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DesktopHomeComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DesktopHomeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
