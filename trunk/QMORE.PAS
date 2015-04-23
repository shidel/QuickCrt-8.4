(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

{ Copyright 2015 Jerome Shidel }

unit QMore; { QuickCRT More QObjects }
{$I QCRT.DEF}

interface

{$DEFINE INTERFACE}

uses 
   {$IFDEF DEVLOG} QLog, {$ENDIF} 
	QCrt, QObjects, QControls;
	
{$I TKeyStroke.inc}
{$I TCharacterMap.Inc}


{$UNDEF INTERFACE}

implementation

{$DEFINE IMPLEMENTATION}

uses QErrors, QFiles, QFmtStr, QStrings;

{$I TKeyStroke.inc}
{$I TCharacterMap.Inc}

{$UNDEF IMPLEMENTATION}
end.
