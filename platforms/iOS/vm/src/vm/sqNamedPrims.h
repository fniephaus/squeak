/* Smalltalk from Squeak4.5 with VMMaker 4.13.12 translated as C source on 28 March 2015 8:15:54 pm */
/* This is an automatically generated table of all builtin modules in the VM */

extern sqExport vm_exports[];
extern sqExport os_exports[];
extern sqExport ADPCMCodecPlugin_exports[];
extern sqExport AsynchFilePlugin_exports[];
extern sqExport BMPReadWriterPlugin_exports[];
extern sqExport B2DPlugin_exports[];
extern sqExport BitBltPlugin_exports[];
extern sqExport DSAPrims_exports[];
extern sqExport DropPlugin_exports[];
extern sqExport FFTPlugin_exports[];
extern sqExport FilePlugin_exports[];
extern sqExport FloatArrayPlugin_exports[];
extern sqExport GeniePlugin_exports[];
extern sqExport JPEGReadWriter2Plugin_exports[];
extern sqExport JPEGReaderPlugin_exports[];
extern sqExport JoystickTabletPlugin_exports[];
extern sqExport Klatt_exports[];
extern sqExport LargeIntegers_exports[];
extern sqExport LocalePlugin_exports[];
extern sqExport Matrix2x3Plugin_exports[];
extern sqExport MiscPrimitivePlugin_exports[];
extern sqExport RePlugin_exports[];
extern sqExport SecurityPlugin_exports[];
extern sqExport SerialPlugin_exports[];
extern sqExport SocketPlugin_exports[];
extern sqExport SoundCodecPrims_exports[];
extern sqExport SoundGenerationPlugin_exports[];
extern sqExport SoundPlugin_exports[];
extern sqExport StarSqueakPlugin_exports[];
extern sqExport SurfacePlugin_exports[];
extern sqExport UUIDPlugin_exports[];

sqExport *pluginExports[] = {
	vm_exports,
	os_exports,
	ADPCMCodecPlugin_exports,
	AsynchFilePlugin_exports,
	BMPReadWriterPlugin_exports,
	B2DPlugin_exports,
	BitBltPlugin_exports,
	DSAPrims_exports,
#ifdef BUILD_FOR_OSX
	DropPlugin_exports,
    JoystickTabletPlugin_exports,
#endif
	FFTPlugin_exports,
	FilePlugin_exports,
	FloatArrayPlugin_exports,
	GeniePlugin_exports,
	JPEGReadWriter2Plugin_exports,
	JPEGReaderPlugin_exports,
	Klatt_exports,
	LargeIntegers_exports,
	LocalePlugin_exports,
	Matrix2x3Plugin_exports,
	MiscPrimitivePlugin_exports,
	RePlugin_exports,
	SecurityPlugin_exports,
#ifdef BUILD_FOR_OSX
	SerialPlugin_exports,
#endif
	SocketPlugin_exports,
	SoundCodecPrims_exports,
	SoundGenerationPlugin_exports,
	SoundPlugin_exports,
	StarSqueakPlugin_exports,
	SurfacePlugin_exports,
    UUIDPlugin_exports,
	NULL
};
