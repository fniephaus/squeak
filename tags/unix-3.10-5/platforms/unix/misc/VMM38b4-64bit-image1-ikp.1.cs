'From Squeak3.8gamma of ''24 November 2004'' [latest update: #6548] on 31 March 2005 at 11:13:48 am'!"Change Set:		VMM38b4-64bit-image1-ikpDate:			2005-03-31Author:			ian.piumarta@squeakland.orgChanges relative to 3.8g-6548 that add 64-bit support to the image.  File this in BEFORE filing in VMM38b4-64bit-image2."!!SystemDictionary methodsFor: 'sources, change log' stamp: 'ikp 3/26/2005 21:59'!wordSize	"Answer the size (in bytes) of an object pointer."	"Smalltalk wordSize"	^[SmalltalkImage current vmParameterAt: 27] on: Error do: [4]! !