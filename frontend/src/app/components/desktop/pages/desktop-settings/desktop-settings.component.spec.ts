import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DesktopSettingsComponent } from './desktop-settings.component';

describe('DesktopSettingsComponent', () => {
  let component: DesktopSettingsComponent;
  let fixture: ComponentFixture<DesktopSettingsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DesktopSettingsComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DesktopSettingsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
