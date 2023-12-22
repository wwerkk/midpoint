Midpoint : UGen {
	*ar { arg freq = 440.0, depth = 1, spread = 0.5, reduction = 0.5;
		^this.multiNew('audio', freq, depth, spread, reduction)
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
