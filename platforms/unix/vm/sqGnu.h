/* Definitions for "gnuified" interp.c
 * 
 *   Copyright (C) 1996-2003 Ian Piumarta and other authors/contributors
 *     as listed elsewhere in this file.
 *   All rights reserved.
 *   
 *     You are NOT ALLOWED to distribute modified versions of this file
 *     under its original name.  If you want to modify it and then make
 *     your modifications available publicly, rename the file first.
 * 
 *   This file is part of Unix Squeak.
 * 
 *   This file is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.
 *   
 *   You may use and/or distribute this file ONLY as part of Squeak, under
 *   the terms of the Squeak License as described in `LICENSE' in the base of
 *   this distribution, subject to the following additional restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software.  If you use this software
 *      in a product, an acknowledgment to the original author(s) (and any
 *      other contributors mentioned herein) in the product documentation
 *      would be appreciated but is not required.
 * 
 *   2. You must not distribute (or make publicly available by any
 *      means) a modified copy of this file unless you first rename it.
 * 
 *   3. This notice must not be removed or altered in any source distribution.
 * 
 *   Using (or modifying this file for use) in any context other than Squeak
 *   changes these copyright conditions.  Read the file `COPYING' in the
 *   directory `platforms/unix/doc' before proceeding with any such use.
 */

/* Author: Ian.Piumarta@inria.fr
 *
 * Last edited: 2003-09-10 06:01:29 by piumarta on emilia.inria.fr
 *
 * NOTES:
 *	this file is #included IN PLACE OF sq.h
 */

#include "sq.h"

#define CASE(N)		case N: _##N:

#if defined(__powerpc__) || defined(PPC) || defined(_POWER) || defined(_IBMR2) || defined(__ppc__)
# define JUMP_TABLE_PTR	; register void **jumpTableP JP_REG; jumpTableP= &jumpTable[0]
# define BREAK		goto *jumpTableP[currentBytecode]
#else
# define JUMP_TABLE_PTR
# define BREAK		goto *jumpTable[currentBytecode]
#endif

#if defined(SQ_USE_GLOBAL_STRUCT)
# define PRIM_DISPATCH	goto *jumpTable[foo->primitiveIndex]
#else
# define PRIM_DISPATCH	goto *jumpTable[primitiveIndex]
#endif

#define JUMP_TABLE \
  static void *jumpTable[256]= { \
      &&_0,   &&_1,   &&_2,   &&_3,   &&_4,   &&_5,   &&_6,   &&_7,   &&_8,   &&_9, \
     &&_10,  &&_11,  &&_12,  &&_13,  &&_14,  &&_15,  &&_16,  &&_17,  &&_18,  &&_19, \
     &&_20,  &&_21,  &&_22,  &&_23,  &&_24,  &&_25,  &&_26,  &&_27,  &&_28,  &&_29, \
     &&_30,  &&_31,  &&_32,  &&_33,  &&_34,  &&_35,  &&_36,  &&_37,  &&_38,  &&_39, \
     &&_40,  &&_41,  &&_42,  &&_43,  &&_44,  &&_45,  &&_46,  &&_47,  &&_48,  &&_49, \
     &&_50,  &&_51,  &&_52,  &&_53,  &&_54,  &&_55,  &&_56,  &&_57,  &&_58,  &&_59, \
     &&_60,  &&_61,  &&_62,  &&_63,  &&_64,  &&_65,  &&_66,  &&_67,  &&_68,  &&_69, \
     &&_70,  &&_71,  &&_72,  &&_73,  &&_74,  &&_75,  &&_76,  &&_77,  &&_78,  &&_79, \
     &&_80,  &&_81,  &&_82,  &&_83,  &&_84,  &&_85,  &&_86,  &&_87,  &&_88,  &&_89, \
     &&_90,  &&_91,  &&_92,  &&_93,  &&_94,  &&_95,  &&_96,  &&_97,  &&_98,  &&_99, \
    &&_100, &&_101, &&_102, &&_103, &&_104, &&_105, &&_106, &&_107, &&_108, &&_109, \
    &&_110, &&_111, &&_112, &&_113, &&_114, &&_115, &&_116, &&_117, &&_118, &&_119, \
    &&_120, &&_121, &&_122, &&_123, &&_124, &&_125, &&_126, &&_127, &&_128, &&_129, \
    &&_130, &&_131, &&_132, &&_133, &&_134, &&_135, &&_136, &&_137, &&_138, &&_139, \
    &&_140, &&_141, &&_142, &&_143, &&_144, &&_145, &&_146, &&_147, &&_148, &&_149, \
    &&_150, &&_151, &&_152, &&_153, &&_154, &&_155, &&_156, &&_157, &&_158, &&_159, \
    &&_160, &&_161, &&_162, &&_163, &&_164, &&_165, &&_166, &&_167, &&_168, &&_169, \
    &&_170, &&_171, &&_172, &&_173, &&_174, &&_175, &&_176, &&_177, &&_178, &&_179, \
    &&_180, &&_181, &&_182, &&_183, &&_184, &&_185, &&_186, &&_187, &&_188, &&_189, \
    &&_190, &&_191, &&_192, &&_193, &&_194, &&_195, &&_196, &&_197, &&_198, &&_199, \
    &&_200, &&_201, &&_202, &&_203, &&_204, &&_205, &&_206, &&_207, &&_208, &&_209, \
    &&_210, &&_211, &&_212, &&_213, &&_214, &&_215, &&_216, &&_217, &&_218, &&_219, \
    &&_220, &&_221, &&_222, &&_223, &&_224, &&_225, &&_226, &&_227, &&_228, &&_229, \
    &&_230, &&_231, &&_232, &&_233, &&_234, &&_235, &&_236, &&_237, &&_238, &&_239, \
    &&_240, &&_241, &&_242, &&_243, &&_244, &&_245, &&_246, &&_247, &&_248, &&_249, \
    &&_250, &&_251, &&_252, &&_253, &&_254, &&_255 \
  } JUMP_TABLE_PTR

#define PRIM_TABLE \
  static void *jumpTable[700]= { \
      &&_0,   &&_1,   &&_2,   &&_3,   &&_4,   &&_5,   &&_6,   &&_7,   &&_8,   &&_9, \
     &&_10,  &&_11,  &&_12,  &&_13,  &&_14,  &&_15,  &&_16,  &&_17,  &&_18,  &&_19, \
     &&_20,  &&_21,  &&_22,  &&_23,  &&_24,  &&_25,  &&_26,  &&_27,  &&_28,  &&_29, \
     &&_30,  &&_31,  &&_32,  &&_33,  &&_34,  &&_35,  &&_36,  &&_37,  &&_38,  &&_39, \
     &&_40,  &&_41,  &&_42,  &&_43,  &&_44,  &&_45,  &&_46,  &&_47,  &&_48,  &&_49, \
     &&_50,  &&_51,  &&_52,  &&_53,  &&_54,  &&_55,  &&_56,  &&_57,  &&_58,  &&_59, \
     &&_60,  &&_61,  &&_62,  &&_63,  &&_64,  &&_65,  &&_66,  &&_67,  &&_68,  &&_69, \
     &&_70,  &&_71,  &&_72,  &&_73,  &&_74,  &&_75,  &&_76,  &&_77,  &&_78,  &&_79, \
     &&_80,  &&_81,  &&_82,  &&_83,  &&_84,  &&_85,  &&_86,  &&_87,  &&_88,  &&_89, \
     &&_90,  &&_91,  &&_92,  &&_93,  &&_94,  &&_95,  &&_96,  &&_97,  &&_98,  &&_99, \
    &&_100, &&_101, &&_102, &&_103, &&_104, &&_105, &&_106, &&_107, &&_108, &&_109, \
    &&_110, &&_111, &&_112, &&_113, &&_114, &&_115, &&_116, &&_117, &&_118, &&_119, \
    &&_120, &&_121, &&_122, &&_123, &&_124, &&_125, &&_126, &&_127, &&_128, &&_129, \
    &&_130, &&_131, &&_132, &&_133, &&_134, &&_135, &&_136, &&_137, &&_138, &&_139, \
    &&_140, &&_141, &&_142, &&_143, &&_144, &&_145, &&_146, &&_147, &&_148, &&_149, \
    &&_150, &&_151, &&_152, &&_153, &&_154, &&_155, &&_156, &&_157, &&_158, &&_159, \
    &&_160, &&_161, &&_162, &&_163, &&_164, &&_165, &&_166, &&_167, &&_168, &&_169, \
    &&_170, &&_171, &&_172, &&_173, &&_174, &&_175, &&_176, &&_177, &&_178, &&_179, \
    &&_180, &&_181, &&_182, &&_183, &&_184, &&_185, &&_186, &&_187, &&_188, &&_189, \
    &&_190, &&_191, &&_192, &&_193, &&_194, &&_195, &&_196, &&_197, &&_198, &&_199, \
    &&_200, &&_201, &&_202, &&_203, &&_204, &&_205, &&_206, &&_207, &&_208, &&_209, \
    &&_210, &&_211, &&_212, &&_213, &&_214, &&_215, &&_216, &&_217, &&_218, &&_219, \
    &&_220, &&_221, &&_222, &&_223, &&_224, &&_225, &&_226, &&_227, &&_228, &&_229, \
    &&_230, &&_231, &&_232, &&_233, &&_234, &&_235, &&_236, &&_237, &&_238, &&_239, \
    &&_240, &&_241, &&_242, &&_243, &&_244, &&_245, &&_246, &&_247, &&_248, &&_249, \
    &&_250, &&_251, &&_252, &&_253, &&_254, &&_255, &&_256, &&_257, &&_258, &&_259, \
    &&_260, &&_261, &&_262, &&_263, &&_264, &&_265, &&_266, &&_267, &&_268, &&_269, \
    &&_270, &&_271, &&_272, &&_273, &&_274, &&_275, &&_276, &&_277, &&_278, &&_279, \
    &&_280, &&_281, &&_282, &&_283, &&_284, &&_285, &&_286, &&_287, &&_288, &&_289, \
    &&_290, &&_291, &&_292, &&_293, &&_294, &&_295, &&_296, &&_297, &&_298, &&_299, \
    &&_300, &&_301, &&_302, &&_303, &&_304, &&_305, &&_306, &&_307, &&_308, &&_309, \
    &&_310, &&_311, &&_312, &&_313, &&_314, &&_315, &&_316, &&_317, &&_318, &&_319, \
    &&_320, &&_321, &&_322, &&_323, &&_324, &&_325, &&_326, &&_327, &&_328, &&_329, \
    &&_330, &&_331, &&_332, &&_333, &&_334, &&_335, &&_336, &&_337, &&_338, &&_339, \
    &&_340, &&_341, &&_342, &&_343, &&_344, &&_345, &&_346, &&_347, &&_348, &&_349, \
    &&_350, &&_351, &&_352, &&_353, &&_354, &&_355, &&_356, &&_357, &&_358, &&_359, \
    &&_360, &&_361, &&_362, &&_363, &&_364, &&_365, &&_366, &&_367, &&_368, &&_369, \
    &&_370, &&_371, &&_372, &&_373, &&_374, &&_375, &&_376, &&_377, &&_378, &&_379, \
    &&_380, &&_381, &&_382, &&_383, &&_384, &&_385, &&_386, &&_387, &&_388, &&_389, \
    &&_390, &&_391, &&_392, &&_393, &&_394, &&_395, &&_396, &&_397, &&_398, &&_399, \
    &&_400, &&_401, &&_402, &&_403, &&_404, &&_405, &&_406, &&_407, &&_408, &&_409, \
    &&_410, &&_411, &&_412, &&_413, &&_414, &&_415, &&_416, &&_417, &&_418, &&_419, \
    &&_420, &&_421, &&_422, &&_423, &&_424, &&_425, &&_426, &&_427, &&_428, &&_429, \
    &&_430, &&_431, &&_432, &&_433, &&_434, &&_435, &&_436, &&_437, &&_438, &&_439, \
    &&_440, &&_441, &&_442, &&_443, &&_444, &&_445, &&_446, &&_447, &&_448, &&_449, \
    &&_450, &&_451, &&_452, &&_453, &&_454, &&_455, &&_456, &&_457, &&_458, &&_459, \
    &&_460, &&_461, &&_462, &&_463, &&_464, &&_465, &&_466, &&_467, &&_468, &&_469, \
    &&_470, &&_471, &&_472, &&_473, &&_474, &&_475, &&_476, &&_477, &&_478, &&_479, \
    &&_480, &&_481, &&_482, &&_483, &&_484, &&_485, &&_486, &&_487, &&_488, &&_489, \
    &&_490, &&_491, &&_492, &&_493, &&_494, &&_495, &&_496, &&_497, &&_498, &&_499, \
    &&_500, &&_501, &&_502, &&_503, &&_504, &&_505, &&_506, &&_507, &&_508, &&_509, \
    &&_510, &&_511, &&_512, &&_513, &&_514, &&_515, &&_516, &&_517, &&_518, &&_519, \
    &&_520, &&_521, &&_522, &&_523, &&_524, &&_525, &&_526, &&_527, &&_528, &&_529, \
    &&_530, &&_531, &&_532, &&_533, &&_534, &&_535, &&_536, &&_537, &&_538, &&_539, \
    &&_540, &&_541, &&_542, &&_543, &&_544, &&_545, &&_546, &&_547, &&_548, &&_549, \
    &&_550, &&_551, &&_552, &&_553, &&_554, &&_555, &&_556, &&_557, &&_558, &&_559, \
    &&_560, &&_561, &&_562, &&_563, &&_564, &&_565, &&_566, &&_567, &&_568, &&_569, \
    &&_570, &&_571, &&_572, &&_573, &&_574, &&_575, &&_576, &&_577, &&_578, &&_579, \
    &&_580, &&_581, &&_582, &&_583, &&_584, &&_585, &&_586, &&_587, &&_588, &&_589, \
    &&_590, &&_591, &&_592, &&_593, &&_594, &&_595, &&_596, &&_597, &&_598, &&_599, \
    &&_600, &&_601, &&_602, &&_603, &&_604, &&_605, &&_606, &&_607, &&_608, &&_609, \
    &&_610, &&_611, &&_612, &&_613, &&_614, &&_615, &&_616, &&_617, &&_618, &&_619, \
    &&_620, &&_621, &&_622, &&_623, &&_624, &&_625, &&_626, &&_627, &&_628, &&_629, \
    &&_630, &&_631, &&_632, &&_633, &&_634, &&_635, &&_636, &&_637, &&_638, &&_639, \
    &&_640, &&_641, &&_642, &&_643, &&_644, &&_645, &&_646, &&_647, &&_648, &&_649, \
    &&_650, &&_651, &&_652, &&_653, &&_654, &&_655, &&_656, &&_657, &&_658, &&_659, \
    &&_660, &&_661, &&_662, &&_663, &&_664, &&_665, &&_666, &&_667, &&_668, &&_669, \
    &&_670, &&_671, &&_672, &&_673, &&_674, &&_675, &&_676, &&_677, &&_678, &&_679, \
    &&_680, &&_681, &&_682, &&_683, &&_684, &&_685, &&_686, &&_687, &&_688, &&_689, \
    &&_690, &&_691, &&_692, &&_693, &&_694, &&_695, &&_696, &&_697, &&_698, &&_699, \
  }

  /*
     IP_REG, SP_REG, CB_REG
        the machine registers in which to place localIP, localSP and
        currentBytecode.  Wins big on register-deficient architectures --
        especially Intel.
  */
#if defined(__mips__)
# define IP_REG __asm__("$16")
# define SP_REG __asm__("$17")
# define CB_REG __asm__("$18")
#endif
#if defined(__sparc__)
# define IP_REG __asm__("%l0")
# define SP_REG __asm__("%l1")
# define CB_REG __asm__("%l2")
#endif
#if defined(__alpha__)
# define IP_REG __asm__("$9")
# define SP_REG __asm__("$10")
# define CB_REG __asm__("$11")
#endif
#if defined(__i386__)
# define IP_REG __asm__("%esi")
# define SP_REG __asm__("%edi")
# if (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 95))
#   define CB_REG __asm__("%ebx")
# else
#   define CB_REG /* avoid undue register pressure */
# endif
#endif
#if defined(__powerpc__) || defined(PPC) || defined(_POWER) || defined(_IBMR2) || defined(__ppc__)
# define GP_REG __asm__("24")
# define JP_REG __asm__("25")
# define IP_REG __asm__("26")
# define SP_REG __asm__("27")
# define CB_REG __asm__("28")
#endif
#if defined(__hppa__)
# define IP_REG __asm__("%r18")
# define SP_REG __asm__("%r17")
# define CB_REG __asm__("%r16")
#endif
#if defined(__mc68000__)
# define IP_REG __asm__("a5")
# define SP_REG __asm__("a4")
# define CB_REG __asm__("d7")
#endif

#ifndef JP_REG
# define JP_REG
#endif
#ifndef IP_REG
# define IP_REG
#endif
#ifndef SP_REG
# define SP_REG
#endif
#ifndef CB_REG
# define CB_REG
#endif
#ifndef GP_REG
# define GP_REG
#endif
