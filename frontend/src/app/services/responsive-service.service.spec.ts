import { TestBed } from '@angular/core/testing';

import { ResponsiveServiceService } from './responsive-service.service';

describe('ResponsiveServiceService', () => {
  let service: ResponsiveServiceService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ResponsiveServiceService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
