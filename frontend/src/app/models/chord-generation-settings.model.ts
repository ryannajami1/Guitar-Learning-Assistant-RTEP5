export interface ChordGenerationSettings {
  notes: {
    A: boolean;
    B: boolean;
    C: boolean;
    D: boolean;
    E: boolean;
    F: boolean;
    G: boolean;
  };
  types: {
    standard: boolean;
    maj: boolean;
    min: boolean;
    seven: boolean;
    maj7: boolean;
    min7: boolean;
    sus4: boolean;
    dim: boolean;
    aug: boolean;
  };
}

export const defaultChordGenerationSettings: ChordGenerationSettings = {
  notes: {
    A: true,
    B: true,
    C: true,
    D: true,
    E: true,
    F: true,
    G: true,
  },
  types: {
    standard: true,
    maj: true,
    min: true,
    seven: true,
    maj7: true,
    min7: true,
    sus4: true,
    dim: true,
    aug: true,
  },
};
