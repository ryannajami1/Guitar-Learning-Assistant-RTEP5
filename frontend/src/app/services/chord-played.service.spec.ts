import { TestBed } from '@angular/core/testing';

import { ChordPlayedService } from './chord-played.service';

describe('ChordPlayedService', () => {
  let service: ChordPlayedService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ChordPlayedService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
