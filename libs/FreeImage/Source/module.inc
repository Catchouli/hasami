set (fi_root_src
	./FreeImage/BitmapAccess.cpp
	./FreeImage/ColorLookup.cpp
	./FreeImage/FreeImage.cpp
	./FreeImage/FreeImageC.c
	./FreeImage/FreeImageIO.cpp
	./FreeImage/GetType.cpp
	./FreeImage/MemoryIO.cpp
	./FreeImage/PixelAccess.cpp
	./FreeImage/MNGHelper.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage" FILES ${fi_root_src})

set (fi_root_conversion_src
	./FreeImage/Conversion.cpp
	./FreeImage/Conversion16_555.cpp
	./FreeImage/Conversion16_565.cpp
	./FreeImage/Conversion24.cpp
	./FreeImage/Conversion32.cpp
	./FreeImage/Conversion4.cpp
	./FreeImage/Conversion8.cpp
	./FreeImage/ConversionFloat.cpp
	./FreeImage/ConversionRGBF.cpp
	./FreeImage/ConversionRGB16.cpp
	./FreeImage/ConversionRGBA16.cpp
	./FreeImage/ConversionRGBAF.cpp
	./FreeImage/ConversionType.cpp
	./FreeImage/ConversionUINT16.cpp
	./FreeImage/Halftoning.cpp
	./FreeImage/tmoColorConvert.cpp
	./FreeImage/tmoDrago03.cpp
	./FreeImage/tmoFattal02.cpp
	./FreeImage/tmoReinhard05.cpp
	./FreeImage/ToneMapping.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage\\Conversion" FILES ${fi_root_conversion_src})

set (fi_root_deprecation_src
	./DeprecationManager/Deprecated.cpp
	./DeprecationManager/DeprecationMgr.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage\\DeprecationMgr" FILES ${fi_root_deprecation_src})

set (fi_root_metadata_src
	./Metadata/Exif.cpp
	./Metadata/FIRational.cpp
	./Metadata/FreeImageTag.cpp
	./Metadata/IPTC.cpp
	./Metadata/TagConversion.cpp
	./Metadata/TagLib.cpp
	./Metadata/XTIFF.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage\\Metadata" FILES ${fi_root_metadata_src})

set (fi_root_multipaging_src
	./FreeImage/CacheFile.cpp
	./FreeImage/MultiPage.cpp
	./FreeImage/ZLibInterface.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage\\MultiPaging" FILES ${fi_root_multipaging_src})

set (fi_root_plugins_src
	./FreeImage/J2KHelper.cpp
	./FreeImage/Plugin.cpp
	./FreeImage/PluginBMP.cpp
	./FreeImage/PluginCUT.cpp
	./FreeImage/PluginDDS.cpp
	./FreeImage/PluginG3.cpp
	./FreeImage/PluginGIF.cpp
	./FreeImage/PluginHDR.cpp
	./FreeImage/PluginICO.cpp
	./FreeImage/PluginIFF.cpp
	./FreeImage/PluginJ2K.cpp
	./FreeImage/PluginJP2.cpp
	./FreeImage/PluginJPEG.cpp
	./FreeImage/PluginKOALA.cpp
	./FreeImage/PluginMNG.cpp
	./FreeImage/PluginPCD.cpp
	./FreeImage/PluginPCX.cpp
	./FreeImage/PluginPFM.cpp
	./FreeImage/PluginPICT.cpp
	./FreeImage/PluginPNG.cpp
	./FreeImage/PluginPNM.cpp
	./FreeImage/PluginPSD.cpp
	./FreeImage/PluginRAS.cpp
	./FreeImage/PluginRAW.cpp
	./FreeImage/PluginSGI.cpp
	./FreeImage/PluginTARGA.cpp
	./FreeImage/PluginTIFF.cpp
	./FreeImage/PluginWBMP.cpp
	./FreeImage/PluginXBM.cpp
	./FreeImage/PluginXPM.cpp
	./FreeImage/PSDParser.cpp
	./FreeImage/TIFFLogLuv.cpp
	)

if (NOT EMCC)
set (fi_root_plugins_src ${fi_root_plugins_src} 
	./FreeImage/PluginEXR.cpp
	./FreeImage/PluginJNG.cpp
	./FreeImage/PluginWebP.cpp
	./FreeImage/PluginJXR.cpp
    )
else (NOT EMCC)
  add_definitions(-DDISABLE_PLUGIN_FOR_EMCC)
endif (NOT EMCC)

SOURCE_GROUP("Source Files\\FreeImage\\Plugins" FILES ${fi_root_plugins_src})

set (fi_root_quantizers_src
	./FreeImage/NNQuantizer.cpp
	./FreeImage/WuQuantizer.cpp
	./FreeImage/LFPQuantizer.cpp
	)

SOURCE_GROUP("Source Files\\FreeImage\\Quantizers" FILES ${fi_root_quantizers_src})

set (fi_root_toolkit_src
	./FreeImageToolkit/Background.cpp
	./FreeImageToolkit/BSplineRotate.cpp
	./FreeImageToolkit/Channels.cpp
	./FreeImageToolkit/ClassicRotate.cpp
	./FreeImageToolkit/Colors.cpp
	./FreeImageToolkit/CopyPaste.cpp
	./FreeImageToolkit/Display.cpp
	./FreeImageToolkit/Flip.cpp
	./FreeImageToolkit/JPEGTransform.cpp
	./FreeImageToolkit/MultigridPoissonSolver.cpp
	./FreeImageToolkit/Rescale.cpp
	./FreeImageToolkit/Resize.cpp
	./FreeImageToolkit/Resize.h
	)

SOURCE_GROUP("Source Files\\FreeImage\\Toolkit" FILES ${fi_root_toolkit_src})

set (fi_plus_src
	#../Wrapper/FreeImagePlus/src/fipImage.cpp
	#../Wrapper/FreeImagePlus/src/fipMemoryIO.cpp
	#../Wrapper/FreeImagePlus/src/fipMetadataFind.cpp
	#../Wrapper/FreeImagePlus/src/fipMultiPage.cpp
	#../Wrapper/FreeImagePlus/src/fipTag.cpp
	#../Wrapper/FreeImagePlus/src/fipWinImage.cpp
	)

SOURCE_GROUP("Source Files\\FreeImagePlus" FILES ${fi_plus_src})

set (fi_plugin_jpeg_src
	./LibJPEG/jaricom.c
	./LibJPEG/jcapimin.c
	./LibJPEG/jcapistd.c
	./LibJPEG/jcarith.c
	./LibJPEG/jccoefct.c
	./LibJPEG/jccolor.c
	./LibJPEG/jcdctmgr.c
	./LibJPEG/jchuff.c
	./LibJPEG/jcinit.c
	./LibJPEG/jcmainct.c
	./LibJPEG/jcmarker.c
	./LibJPEG/jcmaster.c
	./LibJPEG/jcomapi.c
	./LibJPEG/jcparam.c
	./LibJPEG/jcprepct.c
	./LibJPEG/jcsample.c
	./LibJPEG/jctrans.c
	./LibJPEG/jdapimin.c
	./LibJPEG/jdapistd.c
	./LibJPEG/jdarith.c
	./LibJPEG/jdatadst.c
	./LibJPEG/jdatasrc.c
	./LibJPEG/jdcoefct.c
	./LibJPEG/jdcolor.c
	./LibJPEG/jddctmgr.c
	./LibJPEG/jdhuff.c
	./LibJPEG/jdinput.c
	./LibJPEG/jdmainct.c
	./LibJPEG/jdmarker.c
	./LibJPEG/jdmaster.c
	./LibJPEG/jdmerge.c
	./LibJPEG/jdpostct.c
	./LibJPEG/jdsample.c
	./LibJPEG/jdtrans.c
	./LibJPEG/jerror.c
	./LibJPEG/jfdctflt.c
	./LibJPEG/jfdctfst.c
	./LibJPEG/jfdctint.c
	./LibJPEG/jidctflt.c
	./LibJPEG/jidctfst.c
	./LibJPEG/jidctint.c
	./LibJPEG/jmemmgr.c
	./LibJPEG/jmemnobs.c
	./LibJPEG/jquant1.c
	./LibJPEG/jquant2.c
	./LibJPEG/jutils.c
	./LibJPEG/transupp.c
	)

SOURCE_GROUP("Source Files\\LibJPEG" FILES ${fi_plugin_jpeg_src})

set (fi_plugin_jpeg_include
	./LibJPEG/jversion.h
	./LibJPEG/jmorecfg.h
	./LibJPEG/transupp.h
	./LibJPEG/jmemsys.h
	./LibJPEG/jdct.h
	./LibJPEG/jconfig.h
	./LibJPEG/jinclude.h
	./LibJPEG/jpeglib.h
	./LibJPEG/cderror.h
	./LibJPEG/jpegint.h
	./LibJPEG/cdjpeg.h
	./LibJPEG/jerror.h
	)

SOURCE_GROUP("Header Files\\LibJPEG" FILES ${fi_plugin_jpeg_include})

set (fi_plugin_mng_src
	)

SOURCE_GROUP("Source Files\\LibMNG" FILES ${fi_plugin_mng_src})

set (fi_plugin_mng_include
	)

SOURCE_GROUP("Header Files\\LibMNG" FILES ${fi_plugin_mng_include})

if (NOT EMCC)
set (fi_plugin_jxr_src
	./LibJXR/image/decode/decode.c
	./LibJXR/image/decode/JXRTranscode.c
	./LibJXR/image/decode/postprocess.c
	./LibJXR/image/decode/segdec.c
	./LibJXR/image/decode/strdec.c
	./LibJXR/image/decode/strdec_x86.c
	./LibJXR/image/decode/strInvTransform.c
	./LibJXR/image/decode/strPredQuantDec.c
	./LibJXR/image/encode/encode.c
	./LibJXR/image/encode/segenc.c
	./LibJXR/image/encode/strenc.c
	./LibJXR/image/encode/strenc_x86.c
	./LibJXR/image/encode/strFwdTransform.c
	./LibJXR/image/encode/strPredQuantEnc.c
	./LibJXR/image/sys/adapthuff.c
	./LibJXR/image/sys/image.c
	./LibJXR/image/sys/perfTimerANSI.c
	./LibJXR/image/sys/strcodec.c
	./LibJXR/image/sys/strPredQuant.c
	./LibJXR/image/sys/strTransform.c
	./LibJXR/jxrgluelib/JXRGlue.c
	./LibJXR/jxrgluelib/JXRGlueJxr.c
	./LibJXR/jxrgluelib/JXRGluePFC.c
	./LibJXR/jxrgluelib/JXRMeta.c
	)

SOURCE_GROUP("Source Files\\LibJXR" FILES ${fi_plugin_jxr_src})

set (fi_plugin_jxr_include
	./LibJXR/common/include/guiddef.h
	./LibJXR/common/include/wmsal.h
	./LibJXR/common/include/wmspecstring.h
	./LibJXR/common/include/wmspecstrings_adt.h
	./LibJXR/common/include/wmspecstrings_strict.h
	./LibJXR/common/include/wmspecstrings_undef.h
	./LibJXR/image/decode/decode.h
	./LibJXR/image/encode/encode.h
	./LibJXR/image/sys/ansi.h
	./LibJXR/image/sys/common.h
	./LibJXR/image/sys/perfTimer.h
	./LibJXR/image/sys/strcodec.h
	./LibJXR/image/sys/strTransform.h
	./LibJXR/image/sys/windowsmediaphoto.h
	./LibJXR/image/sys/xplatform_image.h
	./LibJXR/image/x86/x86.h
	./LibJXR/jxrgluelib/JXRGlue.h
	./LibJXR/jxrgluelib/JXRMeta.h
	)

SOURCE_GROUP("Header Files\\LibJXR" FILES ${fi_plugin_jxr_include})
endif(NOT EMCC)

set (fi_plugin_openjpeg_src
	./LibOpenJPEG/bio.c
	./LibOpenJPEG/cio.c
	./LibOpenJPEG/dwt.c
	./LibOpenJPEG/event.c
	./LibOpenJPEG/image.c
	./LibOpenJPEG/j2k.c
	./LibOpenJPEG/jp2.c
	./LibOpenJPEG/mct.c
	./LibOpenJPEG/mqc.c
	./LibOpenJPEG/openjpeg.c
	./LibOpenJPEG/pi.c
	./LibOpenJPEG/raw.c
	./LibOpenJPEG/t1.c
	./LibOpenJPEG/t2.c
	./LibOpenJPEG/tcd.c
	./LibOpenJPEG/tgt.c
	./LibOpenJPEG/function_list.c
	./LibOpenJPEG/invert.c
	./LibOpenJPEG/opj_clock.c
	)

SOURCE_GROUP("Source Files\\LibOpenJPEG" FILES ${fi_plugin_openjpeg_src})

set (fi_plugin_openjpeg_include
	./LibOpenJPEG/bio.h
	./LibOpenJPEG/tgt.h
	./LibOpenJPEG/t1.h
	./LibOpenJPEG/j2k.h
	./LibOpenJPEG/t1_luts.h
	./LibOpenJPEG/cio.h
	./LibOpenJPEG/pi.h
	./LibOpenJPEG/event.h
	./LibOpenJPEG/opj_includes.h
	./LibOpenJPEG/opj_malloc.h
	./LibOpenJPEG/image.h
	./LibOpenJPEG/raw.h
	./LibOpenJPEG/mct.h
	./LibOpenJPEG/opj_config.h
	./LibOpenJPEG/opj_config_private.h
	./LibOpenJPEG/opj_clock.h
#	./LibOpenJPEG/opj_codec.h
	./LibOpenJPEG/jp2.h
	./LibOpenJPEG/t2.h
	./LibOpenJPEG/mqc.h
	./LibOpenJPEG/tcd.h
	./LibOpenJPEG/openjpeg.h
	./LibOpenJPEG/dwt.h
#	./LibOpenJPEG/cidx_manager.h
	./LibOpenJPEG/function_list.h
	./LibOpenJPEG/indexbox_manager.h
	./LibOpenJPEG/invert.h
	./LibOpenJPEG/opj_intmath.h
	./LibOpenJPEG/opj_inttypes.h
	./LibOpenJPEG/opj_stdint.h
	)

SOURCE_GROUP("Header Files\\LibOpenJPEG" FILES ${fi_plugin_openjpeg_include})

set (fi_plugin_png_src
	./LibPNG/png.c
	./LibPNG/pngerror.c
	./LibPNG/pngget.c
	./LibPNG/pngmem.c
	./LibPNG/pngpread.c
	./LibPNG/pngread.c
	./LibPNG/pngrio.c
	./LibPNG/pngrtran.c
	./LibPNG/pngrutil.c
	./LibPNG/pngset.c
	./LibPNG/pngtrans.c
	./LibPNG/pngwio.c
	./LibPNG/pngwrite.c
	./LibPNG/pngwtran.c
	./LibPNG/pngwutil.c
	)

SOURCE_GROUP("Source Files\\LibPNG" FILES ${fi_plugin_png_src})

set (fi_plugin_png_include
	./LibPNG/png.h
	./LibPNG/pngpriv.h
	./LibPNG/pngconf.h
	./LibPNG/pngdebug.h
	./LibPNG/pnginfo.h
	./LibPNG/pnglibconf.h
	./LibPNG/pngstruct.h
	)

SOURCE_GROUP("Header Files\\LibPNG" FILES ${fi_plugin_png_include})

set (fi_plugin_raw_src
#	./LibRawLite/internal/aahd_demosaic.cpp
#	./LibRawLite/internal/dht_demosaic.cpp
#	./LibRawLite/internal/libraw_x3f.cpp
#	./LibRawLite/internal/wf_filtering.cpp
	./LibRawLite/internal/dcraw_common.cpp
	./LibRawLite/internal/dcraw_fileio.cpp
	./LibRawLite/internal/demosaic_packs.cpp
	./LibRawLite/src/libraw_c_api.cpp
	./LibRawLite/src/libraw_cxx.cpp
	./LibRawLite/src/libraw_datastream.cpp
	)

SOURCE_GROUP("Source Files\\LibRaw" FILES ${fi_plugin_raw_src})

set (fi_plugin_raw_include
	./LibRawLite/libraw/libraw_types.h
	./LibRawLite/libraw/libraw_version.h
	./LibRawLite/libraw/libraw_const.h
	./LibRawLite/libraw/libraw_alloc.h
	./LibRawLite/libraw/libraw_datastream.h
	./LibRawLite/libraw/libraw_internal.h
	./LibRawLite/libraw/libraw.h
	./LibRawLite/internal/defines.h
	./LibRawLite/internal/var_defines.h
	./LibRawLite/internal/libraw_internal_funcs.h
	)

SOURCE_GROUP("Header Files\\LibRaw" FILES ${fi_plugin_raw_include})

set (fi_plugin_tiff_src
	./LibTIFF4/tif_aux.c
	./LibTIFF4/tif_close.c
	./LibTIFF4/tif_codec.c
	./LibTIFF4/tif_color.c
	./LibTIFF4/tif_compress.c
	./LibTIFF4/tif_dir.c
	./LibTIFF4/tif_dirinfo.c
	./LibTIFF4/tif_dirread.c
	./LibTIFF4/tif_dirwrite.c
	./LibTIFF4/tif_dumpmode.c
	./LibTIFF4/tif_error.c
	./LibTIFF4/tif_extension.c
	./LibTIFF4/tif_fax3.c
	./LibTIFF4/tif_fax3sm.c
	./LibTIFF4/tif_flush.c
	./LibTIFF4/tif_getimage.c
	./LibTIFF4/tif_jpeg.c
	./LibTIFF4/tif_luv.c
	./LibTIFF4/tif_lzw.c
	./LibTIFF4/tif_next.c
	./LibTIFF4/tif_ojpeg.c
	./LibTIFF4/tif_open.c
	./LibTIFF4/tif_packbits.c
	./LibTIFF4/tif_pixarlog.c
	./LibTIFF4/tif_predict.c
	./LibTIFF4/tif_print.c
	./LibTIFF4/tif_read.c
	./LibTIFF4/tif_strip.c
	./LibTIFF4/tif_swab.c
	./LibTIFF4/tif_thunder.c
	./LibTIFF4/tif_tile.c
	./LibTIFF4/tif_version.c
	./LibTIFF4/tif_warning.c
	./LibTIFF4/tif_write.c
	./LibTIFF4/tif_zip.c
	)

SOURCE_GROUP("Source Files\\LibTIFF4" FILES ${fi_plugin_tiff_src})

set (fi_plugin_tiff_include
	./LibTIFF4/tiff.h
	./LibTIFF4/tif_config.vc.h
	./LibTIFF4/tiffvers.h
	./LibTIFF4/tif_dir.h
	./LibTIFF4/tif_predict.h
	./LibTIFF4/tif_config.wince.h
	./LibTIFF4/tiffconf.vc.h
	./LibTIFF4/tiffconf.wince.h
	./LibTIFF4/t4.h
	./LibTIFF4/tiffiop.h
	./LibTIFF4/uvcode.h
	./LibTIFF4/tiffio.h
	./LibTIFF4/tif_config.h
	./LibTIFF4/tif_fax3.h
	)

SOURCE_GROUP("Header Files\\LibTIFF4" FILES ${fi_plugin_tiff_include})

if (NOT EMCC)
set (fi_plugin_openexr_src
	./OpenEXR/IlmImf/ImfAttribute.cpp
	./OpenEXR/IlmImf/ImfB44Compressor.cpp
	./OpenEXR/IlmImf/ImfBoxAttribute.cpp
	./OpenEXR/IlmImf/ImfChannelList.cpp
	./OpenEXR/IlmImf/ImfChannelListAttribute.cpp
	./OpenEXR/IlmImf/ImfChromaticities.cpp
	./OpenEXR/IlmImf/ImfChromaticitiesAttribute.cpp
	./OpenEXR/IlmImf/ImfCompressionAttribute.cpp
	./OpenEXR/IlmImf/ImfCompressor.cpp
	./OpenEXR/IlmImf/ImfConvert.cpp
	./OpenEXR/IlmImf/ImfCRgbaFile.cpp
	./OpenEXR/IlmImf/ImfDoubleAttribute.cpp
	./OpenEXR/IlmImf/ImfEnvmap.cpp
	./OpenEXR/IlmImf/ImfEnvmapAttribute.cpp
	./OpenEXR/IlmImf/ImfFloatAttribute.cpp
	./OpenEXR/IlmImf/ImfFrameBuffer.cpp
	./OpenEXR/IlmImf/ImfFramesPerSecond.cpp
	./OpenEXR/IlmImf/ImfHeader.cpp
	./OpenEXR/IlmImf/ImfHuf.cpp
        ./OpenEXR/IlmImf/ImfThreading.cpp
	./OpenEXR/IlmImf/ImfInputFile.cpp
	./OpenEXR/IlmImf/ImfIntAttribute.cpp
	./OpenEXR/IlmImf/ImfIO.cpp
	./OpenEXR/IlmImf/ImfRle.cpp
	./OpenEXR/IlmImf/ImfPartType.cpp
	./OpenEXR/IlmImf/ImfOutputPart.cpp
	./OpenEXR/IlmImf/ImfOutputPartData.cpp
	./OpenEXR/IlmImf/ImfKeyCode.cpp
	./OpenEXR/IlmImf/ImfKeyCodeAttribute.cpp
	./OpenEXR/IlmImf/ImfLineOrderAttribute.cpp
	./OpenEXR/IlmImf/ImfLut.cpp
	./OpenEXR/IlmImf/ImfMatrixAttribute.cpp
	./OpenEXR/IlmImf/ImfMisc.cpp
	./OpenEXR/IlmImf/ImfOpaqueAttribute.cpp
	./OpenEXR/IlmImf/ImfOutputFile.cpp
	./OpenEXR/IlmImf/ImfPizCompressor.cpp
	./OpenEXR/IlmImf/ImfPreviewImage.cpp
	./OpenEXR/IlmImf/ImfPreviewImageAttribute.cpp
	./OpenEXR/IlmImf/ImfPxr24Compressor.cpp
	./OpenEXR/IlmImf/ImfRational.cpp
	./OpenEXR/IlmImf/ImfRationalAttribute.cpp
	./OpenEXR/IlmImf/ImfRgbaFile.cpp
	./OpenEXR/IlmImf/ImfRgbaYca.cpp
	./OpenEXR/IlmImf/ImfRleCompressor.cpp
	./OpenEXR/IlmImf/ImfScanLineInputFile.cpp
	./OpenEXR/IlmImf/ImfStandardAttributes.cpp
	./OpenEXR/IlmImf/ImfStdIO.cpp
	./OpenEXR/IlmImf/ImfSystemSpecific.cpp
	./OpenEXR/IlmImf/ImfStringAttribute.cpp
	./OpenEXR/IlmImf/ImfStringVectorAttribute.cpp
	./OpenEXR/IlmImf/ImfTestFile.cpp
	./OpenEXR/IlmImf/ImfTileDescriptionAttribute.cpp
	./OpenEXR/IlmImf/ImfTiledInputFile.cpp
	./OpenEXR/IlmImf/ImfTiledMisc.cpp
	./OpenEXR/IlmImf/ImfTiledOutputFile.cpp
	./OpenEXR/IlmImf/ImfTiledRgbaFile.cpp
	./OpenEXR/IlmImf/ImfTileOffsets.cpp
	./OpenEXR/IlmImf/ImfTiledOutputPart.cpp
	./OpenEXR/IlmImf/ImfTiledInputPart.cpp
	./OpenEXR/IlmImf/ImfTimeCode.cpp
	./OpenEXR/IlmImf/ImfTimeCodeAttribute.cpp
	./OpenEXR/IlmImf/ImfVecAttribute.cpp
	./OpenEXR/IlmImf/ImfVersion.cpp
	./OpenEXR/IlmImf/ImfWav.cpp
	./OpenEXR/IlmImf/ImfZipCompressor.cpp
	./OpenEXR/IlmImf/ImfZip.cpp
	./OpenEXR/IlmImf/ImfMultiPartOutputFile.cpp
	./OpenEXR/IlmImf/ImfMultiPartInputFile.cpp
	./OpenEXR/IlmImf/ImfInputPartData.cpp
	./OpenEXR/IlmImf/ImfInputPart.cpp
	./OpenEXR/IlmImf/ImfFloatVectorAttribute.cpp
	./OpenEXR/IlmImf/ImfGenericInputFile.cpp
	./OpenEXR/IlmImf/ImfGenericOutputFile.cpp
	./OpenEXR/IlmImf/ImfCompositeDeepScanLine.cpp
	./OpenEXR/IlmImf/ImfDeepCompositing.cpp
	./OpenEXR/IlmImf/ImfDeepFrameBuffer.cpp
	./OpenEXR/IlmImf/ImfDeepImageStateAttribute.cpp
	./OpenEXR/IlmImf/ImfDeepScanLineInputFile.cpp
	./OpenEXR/IlmImf/ImfDeepScanLineInputPart.cpp
	./OpenEXR/IlmImf/ImfDeepScanLineOutputFile.cpp
	./OpenEXR/IlmImf/ImfDeepScanLineOutputPart.cpp
	./OpenEXR/IlmImf/ImfDeepTiledInputFile.cpp
	./OpenEXR/IlmImf/ImfDeepTiledInputPart.cpp
	./OpenEXR/IlmImf/ImfDeepTiledOutputFile.cpp
	./OpenEXR/IlmImf/ImfDeepTiledOutputPart.cpp
	./OpenEXR/IlmImf/ImfFastHuf.cpp
	./OpenEXR/Imath/ImathBox.cpp
	./OpenEXR/Imath/ImathColorAlgo.cpp
	./OpenEXR/Imath/ImathFun.cpp
	./OpenEXR/Imath/ImathMatrixAlgo.cpp
	./OpenEXR/Imath/ImathRandom.cpp
	./OpenEXR/Imath/ImathShear.cpp
	./OpenEXR/Imath/ImathVec.cpp
	./OpenEXR/Iex/IexBaseExc.cpp
	./OpenEXR/Iex/IexThrowErrnoExc.cpp
	./OpenEXR/IexMath/IexMathFloatExc.cpp
	./OpenEXR/IexMath/IexMathFpu.cpp
	./OpenEXR/Half/half.cpp
	./OpenEXR/IlmThread/IlmThread.cpp
	./OpenEXR/IlmThread/IlmThreadMutex.cpp
	./OpenEXR/IlmThread/IlmThreadPool.cpp
	./OpenEXR/IlmThread/IlmThreadSemaphore.cpp
	)


if(NOT EMCC)
set (fi_plugin_openexr_src ${fi_plugin_openexr_src}
	./OpenEXR/IlmImf/ImfDwaCompressor.cpp
  )
endif(NOT EMCC)

SOURCE_GROUP("Source Files\\LibOpenEXR" FILES ${fi_plugin_openexr_src})

set (fi_plugin_openexr_include
	./OpenEXR/Half/halfFunction.h
	./OpenEXR/Half/toFloat.h
	./OpenEXR/Half/halfLimits.h
	./OpenEXR/Half/eLut.h
	./OpenEXR/Half/half.h
#	./OpenEXR/Half/halfExport.h
	./OpenEXR/IlmImf/ImfRationalAttribute.h
	./OpenEXR/IlmImf/b44ExpLogTable.h
	./OpenEXR/IlmImf/ImfTiledOutputFile.h
	./OpenEXR/IlmImf/ImfName.h
	./OpenEXR/IlmImf/ImfHeader.h
	./OpenEXR/IlmImf/ImfKeyCode.h
	./OpenEXR/IlmImf/ImfTileDescription.h
	./OpenEXR/IlmImf/ImfKeyCodeAttribute.h
	./OpenEXR/IlmImf/ImfLineOrder.h
	./OpenEXR/IlmImf/ImfVecAttribute.h
	./OpenEXR/IlmImf/ImfPxr24Compressor.h
	./OpenEXR/IlmImf/ImfStdIO.h
	./OpenEXR/IlmImf/ImfTestFile.h
	./OpenEXR/IlmImf/ImfScanLineInputFile.h
	./OpenEXR/IlmImf/ImfSystemSpecific.h
	./OpenEXR/IlmImf/ImfRleCompressor.h
	./OpenEXR/IlmImf/ImfPixelType.h
	./OpenEXR/IlmImf/ImfRgbaYca.h
	./OpenEXR/IlmImf/ImfTileOffsets.h
	./OpenEXR/IlmImf/ImfCheckedArithmetic.h
	./OpenEXR/IlmImf/ImfLut.h
	./OpenEXR/IlmImf/ImfSimd.h
	./OpenEXR/IlmImf/ImfRle.h
	./OpenEXR/IlmImf/ImfPartType.h
	./OpenEXR/IlmImf/ImfPartHelper.h
	./OpenEXR/IlmImf/ImfOutputStreamMutex.h
	./OpenEXR/IlmImf/ImfOutputPartData.h
	./OpenEXR/IlmImf/ImfOutputPart.h
	./OpenEXR/IlmImf/ImfNamespace.h
	./OpenEXR/IlmImf/ImfMultiPartOutputFile.h
	./OpenEXR/IlmImf/ImfMultiPartInputFile.h
	./OpenEXR/IlmImf/ImfOptimizedPixelReading.h
	./OpenEXR/IlmImf/ImfCompressionAttribute.h
	./OpenEXR/IlmImf/ImfStringAttribute.h
	./OpenEXR/IlmImf/ImfTiledMisc.h
	./OpenEXR/IlmImf/ImfPreviewImage.h
	./OpenEXR/IlmImf/ImfCompression.h
	./OpenEXR/IlmImf/ImfOutputFile.h
	./OpenEXR/IlmImf/ImfStandardAttributes.h
	./OpenEXR/IlmImf/ImfVersion.h
	./OpenEXR/IlmImf/ImfTimeCodeAttribute.h
	./OpenEXR/IlmImf/ImfTiledOutputPart.h
	./OpenEXR/IlmImf/ImfTiledInputFile.h
	./OpenEXR/IlmImf/ImfTiledInputPart.h
	./OpenEXR/IlmImf/ImfLineOrderAttribute.h
	./OpenEXR/IlmImf/ImfBoxAttribute.h
	./OpenEXR/IlmImf/ImfEnvmapAttribute.h
	./OpenEXR/IlmImf/ImfRgbaFile.h
	./OpenEXR/IlmImf/ImfMatrixAttribute.h
	./OpenEXR/IlmImf/ImfEnvmap.h
	./OpenEXR/IlmImf/ImfChannelList.h
	./OpenEXR/IlmImf/ImfInputFile.h
	./OpenEXR/IlmImf/ImfFramesPerSecond.h
	./OpenEXR/IlmImf/ImfAttribute.h
	./OpenEXR/IlmImf/ImfTiledRgbaFile.h
	./OpenEXR/IlmImf/ImfFrameBuffer.h
	./OpenEXR/IlmImf/ImfChromaticities.h
	./OpenEXR/IlmImf/ImfCompressor.h
	./OpenEXR/IlmImf/ImfMisc.h
	./OpenEXR/IlmImf/ImfOpaqueAttribute.h
	./OpenEXR/IlmImf/ImfIO.h
	./OpenEXR/IlmImf/ImfThreading.h
	./OpenEXR/IlmImf/ImfInt64.h
	./OpenEXR/IlmImf/ImfMultiView.h
	./OpenEXR/IlmImf/ImfIntAttribute.h
	./OpenEXR/IlmImf/ImfPizCompressor.h
	./OpenEXR/IlmImf/ImfWav.h
	./OpenEXR/IlmImf/ImfFloatAttribute.h
	./OpenEXR/IlmImf/ImfAutoArray.h
	./OpenEXR/IlmImf/ImfRgba.h
	./OpenEXR/IlmImf/ImfPreviewImageAttribute.h
	./OpenEXR/IlmImf/ImfXdr.h
	./OpenEXR/IlmImf/ImfArray.h
	./OpenEXR/IlmImf/ImfChannelListAttribute.h
	./OpenEXR/IlmImf/ImfConvert.h
	./OpenEXR/IlmImf/ImfTileDescriptionAttribute.h
	./OpenEXR/IlmImf/ImfZip.h
	./OpenEXR/IlmImf/ImfZipCompressor.h
	./OpenEXR/IlmImf/ImfB44Compressor.h
	./OpenEXR/IlmImf/ImfRational.h
	./OpenEXR/IlmImf/ImfDoubleAttribute.h
	./OpenEXR/IlmImf/ImfTimeCode.h
	./OpenEXR/IlmImf/ImfChromaticitiesAttribute.h
	./OpenEXR/IlmImf/ImfAcesFile.h
	./OpenEXR/IlmImf/ImfHuf.h
	./OpenEXR/IlmImf/ImfCRgbaFile.h
	./OpenEXR/IlmImf/ImfStringVectorAttribute.h
	./OpenEXR/IlmImf/ImfInputStreamMutex.h
	./OpenEXR/IlmImf/ImfInputPartData.h
	./OpenEXR/IlmImf/ImfInputPart.h
	./OpenEXR/IlmImf/ImfGenericOutputFile.h
	./OpenEXR/IlmImf/ImfGenericInputFile.h
	./OpenEXR/IlmImf/ImfForward.h
	./OpenEXR/IlmImf/ImfFloatVectorAttribute.h
	./OpenEXR/IlmImf/ImfFastHuf.h
	./OpenEXR/IlmImf/ImfExport.h
	./OpenEXR/IlmImf/ImfDeepTiledOutputPart.h
	./OpenEXR/IlmImf/ImfDeepTiledOutputFile.h
	./OpenEXR/IlmImf/ImfDeepTiledInputPart.h
	./OpenEXR/IlmImf/ImfDeepTiledInputFile.h
	./OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h
	./OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h
	./OpenEXR/IlmImf/ImfDeepScanLineInputPart.h
	./OpenEXR/IlmImf/ImfDeepScanLineInputFile.h
	./OpenEXR/IlmImf/ImfDeepImageStateAttribute.h
	./OpenEXR/IlmImf/ImfDeepImageState.h
	./OpenEXR/IlmImf/ImfDeepFrameBuffer.h
	./OpenEXR/IlmImf/ImfDeepCompositing.h
	./OpenEXR/IlmImf/ImfCompositeDeepScanLine.h
	./OpenEXR/OpenEXRConfig.h
	./OpenEXR/Iex/IexMacros.h
	./OpenEXR/Iex/IexMathExc.h
	./OpenEXR/Iex/Iex.h
	./OpenEXR/Iex/IexThrowErrnoExc.h
	./OpenEXR/Iex/IexErrnoExc.h
	./OpenEXR/Iex/IexBaseExc.h
	./OpenEXR/Iex/IexNamespace.h
	./OpenEXR/Iex/IexForward.h
	./OpenEXR/Iex/IexExport.h
	./OpenEXR/IexMath/IexMathIeeeExc.h
	./OpenEXR/IexMath/IexMathFpu.h
	./OpenEXR/IexMath/IexMathFloatExc.h
	./OpenEXR/Imath/ImathExport.h
	./OpenEXR/Imath/ImathForward.h
	./OpenEXR/Imath/ImathFrustumTest.h
	./OpenEXR/Imath/ImathNamespace.h
	./OpenEXR/Imath/ImathBoxAlgo.h
	./OpenEXR/Imath/ImathMath.h
	./OpenEXR/Imath/ImathPlane.h
	./OpenEXR/Imath/ImathRandom.h
	./OpenEXR/Imath/ImathInterval.h
	./OpenEXR/Imath/ImathLine.h
	./OpenEXR/Imath/ImathLimits.h
	./OpenEXR/Imath/ImathShear.h
	./OpenEXR/Imath/ImathPlatform.h
	./OpenEXR/Imath/ImathBox.h
	./OpenEXR/Imath/ImathFrustum.h
	./OpenEXR/Imath/ImathGL.h
	./OpenEXR/Imath/ImathRoots.h
	./OpenEXR/Imath/ImathLineAlgo.h
	./OpenEXR/Imath/ImathMatrixAlgo.h
	./OpenEXR/Imath/ImathVec.h
	./OpenEXR/Imath/ImathVecAlgo.h
	./OpenEXR/Imath/ImathSphere.h
	./OpenEXR/Imath/ImathGLU.h
	./OpenEXR/Imath/ImathFun.h
	./OpenEXR/Imath/ImathMatrix.h
	./OpenEXR/Imath/ImathExc.h
	./OpenEXR/Imath/ImathHalfLimits.h
	./OpenEXR/Imath/ImathColor.h
	./OpenEXR/Imath/ImathInt64.h
	./OpenEXR/Imath/ImathFrame.h
	./OpenEXR/Imath/ImathQuat.h
	./OpenEXR/Imath/ImathColorAlgo.h
	./OpenEXR/Imath/ImathEuler.h
	./OpenEXR/IlmThread/IlmThread.h
	./OpenEXR/IlmThread/IlmThreadMutex.h
	./OpenEXR/IlmThread/IlmThreadSemaphore.h
	./OpenEXR/IlmThread/IlmThreadPool.h
	./OpenEXR/IlmBaseConfig.h
	)

if(NOT EMCC)
set (fi_plugin_openexr_include ${fi_plugin_openexr_include}
	./OpenEXR/IlmImf/ImfDwaCompressorSimd.h
	./OpenEXR/IlmImf/ImfDwaCompressor.h
	./OpenEXR/IlmImf/dwaLookups.h
  )
endif(NOT EMCC)

SOURCE_GROUP("Header Files\\LibOpenEXR" FILES ${fi_plugin_openexr_include})
else (NOT EMCC)

set (fi_plugin_openexr_src
	./OpenEXR/Half/half.cpp
)

SOURCE_GROUP("Source Files\\LibOpenEXR" FILES ${fi_plugin_openexr_src})
set (fi_plugin_openexr_include 
	./OpenEXR/Half/halfFunction.h
	./OpenEXR/Half/toFloat.h
	./OpenEXR/Half/halfLimits.h
	./OpenEXR/Half/eLut.h
	./OpenEXR/Half/half.h
)

SOURCE_GROUP("Header Files\\LibOpenEXR" FILES ${fi_plugin_openexr_include})

endif (NOT EMCC)

if(NOT EMCC)
#LibWebP
set (fi_plugin_libwebp_include
	./LibWebP/src/dec/alphai.h
	./LibWebP/src/dec/decode_vp8.h
	./LibWebP/src/dec/vp8i.h
	./LibWebP/src/dec/vp8li.h
	./LibWebP/src/dec/webpi.h
	./LibWebP/src/dsp/dsp.h
	./LibWebP/src/dsp/lossless.h
	./LibWebP/src/dsp/mips_macro.h
	./LibWebP/src/dsp/neon.h
	./LibWebP/src/dsp/yuv.h
	./LibWebP/src/dsp/yuv_tables_sse2.h
	./LibWebP/src/enc/backward_references.h
	./LibWebP/src/enc/cost.h
	./LibWebP/src/enc/histogram.h
	./LibWebP/src/enc/vp8enci.h
	./LibWebP/src/enc/vp8li.h
	./LibWebP/src/webp/decode.h
	./LibWebP/src/webp/demux.h
	./LibWebP/src/webp/encode.h
	./LibWebP/src/webp/format_constants.h
	./LibWebP/src/webp/mux.h
	./LibWebP/src/webp/mux_types.h
	./LibWebP/src/webp/types.h
	./LibWebP/src/utils/bit_reader.h
	./LibWebP/src/utils/bit_reader_inl.h
	./LibWebP/src/utils/bit_writer.h
	./LibWebP/src/utils/color_cache.h
	./LibWebP/src/utils/endian_inl.h
	./LibWebP/src/utils/filters.h
	./LibWebP/src/utils/huffman.h
	./LibWebP/src/utils/huffman_encode.h
	./LibWebP/src/utils/quant_levels.h
	./LibWebP/src/utils/quant_levels_dec.h
	./LibWebP/src/utils/random.h
	./LibWebP/src/utils/rescaler.h
	./LibWebP/src/utils/thread.h
	./LibWebP/src/utils/utils.h
	./LibWebP/src/mux/muxi.h
  )
SOURCE_GROUP("Header Files\\LibWebP" FILES ${fi_plugin_libwebp_include})

set (fi_plugin_libwebp_src
	./LibWebP/src/dec/dec.alpha.c
	./LibWebP/src/dec/dec.buffer.c
	./LibWebP/src/dec/dec.frame.c
	./LibWebP/src/dec/dec.idec.c
	./LibWebP/src/dec/dec.io.c
	./LibWebP/src/dec/dec.quant.c
	./LibWebP/src/dec/dec.tree.c
	./LibWebP/src/dec/dec.vp8.c
	./LibWebP/src/dec/dec.vp8l.c
	./LibWebP/src/dec/dec.webp.c
	./LibWebP/src/dsp/dsp.alpha_processing.c
	./LibWebP/src/dsp/dsp.alpha_processing_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.alpha_processing_sse2.c
	./LibWebP/src/dsp/dsp.argb.c
	./LibWebP/src/dsp/dsp.argb_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.argb_sse2.c
	./LibWebP/src/dsp/dsp.cost.c
	./LibWebP/src/dsp/dsp.cost_mips32.c
	./LibWebP/src/dsp/dsp.cost_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.cost_sse2.c
	./LibWebP/src/dsp/dsp.cpu.c
	./LibWebP/src/dsp/dsp.dec.c
	./LibWebP/src/dsp/dsp.dec_clip_tables.c
	./LibWebP/src/dsp/dsp.dec_mips32.c
	./LibWebP/src/dsp/dsp.dec_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.dec_neon.c
	./LibWebP/src/dsp/dsp.dec_sse2.c
	./LibWebP/src/dsp/dsp.enc.c
	./LibWebP/src/dsp/dsp.enc_avx2.c
	./LibWebP/src/dsp/dsp.enc_mips32.c
	./LibWebP/src/dsp/dsp.enc_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.enc_neon.c
	./LibWebP/src/dsp/dsp.enc_sse2.c
	./LibWebP/src/dsp/dsp.filters.c
	./LibWebP/src/dsp/dsp.filters_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.filters_sse2.c
	./LibWebP/src/dsp/dsp.lossless.c
	./LibWebP/src/dsp/dsp.lossless_mips32.c
	./LibWebP/src/dsp/dsp.lossless_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.lossless_neon.c
	./LibWebP/src/dsp/dsp.lossless_sse2.c
	./LibWebP/src/dsp/dsp.rescaler.c
	./LibWebP/src/dsp/dsp.rescaler_mips32.c
	./LibWebP/src/dsp/dsp.rescaler_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.upsampling.c
	./LibWebP/src/dsp/dsp.upsampling_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.upsampling_neon.c
	./LibWebP/src/dsp/dsp.upsampling_sse2.c
	./LibWebP/src/dsp/dsp.yuv.c
	./LibWebP/src/dsp/dsp.yuv_mips32.c
	./LibWebP/src/dsp/dsp.yuv_mips_dsp_r2.c
	./LibWebP/src/dsp/dsp.yuv_sse2.c
	./LibWebP/src/enc/enc.alpha.c
	./LibWebP/src/enc/enc.analysis.c
	./LibWebP/src/enc/enc.backward_references.c
	./LibWebP/src/enc/enc.config.c
	./LibWebP/src/enc/enc.cost.c
	./LibWebP/src/enc/enc.filter.c
	./LibWebP/src/enc/enc.frame.c
	./LibWebP/src/enc/enc.histogram.c
	./LibWebP/src/enc/enc.iterator.c
	./LibWebP/src/enc/enc.near_lossless.c
	./LibWebP/src/enc/enc.picture.c
	./LibWebP/src/enc/enc.picture_csp.c
	./LibWebP/src/enc/enc.picture_psnr.c
	./LibWebP/src/enc/enc.picture_rescale.c
	./LibWebP/src/enc/enc.picture_tools.c
	./LibWebP/src/enc/enc.quant.c
	./LibWebP/src/enc/enc.syntax.c
	./LibWebP/src/enc/enc.token.c
	./LibWebP/src/enc/enc.tree.c
	./LibWebP/src/enc/enc.vp8l.c
	./LibWebP/src/enc/enc.webpenc.c
	./LibWebP/src/utils/utils.bit_reader.c
	./LibWebP/src/utils/utils.bit_writer.c
	./LibWebP/src/utils/utils.color_cache.c
	./LibWebP/src/utils/utils.filters.c
	./LibWebP/src/utils/utils.huffman.c
	./LibWebP/src/utils/utils.huffman_encode.c
	./LibWebP/src/utils/utils.quant_levels.c
	./LibWebP/src/utils/utils.quant_levels_dec.c
	./LibWebP/src/utils/utils.random.c
	./LibWebP/src/utils/utils.rescaler.c
	./LibWebP/src/utils/utils.thread.c
	./LibWebP/src/utils/utils.utils.c
	./LibWebP/src/mux/mux.anim_encode.c
	./LibWebP/src/mux/mux.muxedit.c
	./LibWebP/src/mux/mux.muxinternal.c
	./LibWebP/src/mux/mux.muxread.c
	./LibWebP/src/demux/demux.demux.c
  )
SOURCE_GROUP("Source Files\\LibWebP" FILES ${fi_plugin_libwebp_src})

endif(NOT EMCC)

virtalis_module_sources(
  ${TH_THIRDPARTYRASTER_LIB}
	${fi_root_include}
	${fi_root_src}
	${fi_root_conversion_src}
	${fi_root_deprecation_src}
	${fi_root_metadata_src}
	${fi_root_multipaging_src}
	${fi_root_plugins_src}
	${fi_root_quantizers_src}
	${fi_root_toolkit_src}
	${fi_plus_src}
	${fi_plugin_jpeg_src}
	${fi_plugin_jpeg_include}
	${fi_plugin_mng_src}
	${fi_plugin_mng_include}
	${fi_plugin_openjpeg_src}
	${fi_plugin_openjpeg_include}
	${fi_plugin_png_src}
	${fi_plugin_png_include}
	${fi_plugin_raw_src}
	${fi_plugin_raw_include}
	${fi_plugin_tiff_src}
	${fi_plugin_tiff_include}
	${fi_plugin_openexr_src}
	${fi_plugin_openexr_include}
	${fi_plugin_libwebp_include}
	${fi_plugin_libwebp_src}
	${fi_plugin_jxr_src}
	${fi_plugin_jxr_include}
	)