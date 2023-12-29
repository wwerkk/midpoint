Midpoint : UGen {
	*ar { arg freq = 440.0, depth = 2.0, spread = 0.5, reduction = 0.5, lerp = 1;
		^this.multiNew('audio', freq, depth, spread, reduction, lerp)
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
