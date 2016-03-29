'From Squeak3.2gamma of 15 January 2002 [latest update: #4881] on 21 August 2002 at 10:52:51 pm'!
"Change Set:		RePlugin3-Fixes2
Date:			21 August 2002
Author:			ian.piumarta@inria.fr

Adds a cast to one method in RePlugin to avoid a compiler warning."!


!RePlugin methodsFor: 'rcvr linkage' stamp: 'ikp 8/21/2002 22:40'!
rcvrExtraPtr

	|extraObj|
	self inline: true.
	extraObj _ interpreterProxy fetchPointer: 3 ofObject: rcvr.
	(extraObj = (interpreterProxy nilObject))
		ifTrue: [^ self cCode: '(int) NULL'].
	^self 
		cCoerce:(interpreterProxy arrayValueOf: extraObj)
		to: 'int'.! !

