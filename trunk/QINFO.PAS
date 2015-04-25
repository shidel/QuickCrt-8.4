{ Copyright 2015 Jerome Shidel }

(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

unit QInfo; { QuickCRT Application Informationrmation Object }
{$I QCRT.DEF}

interface

uses QClass, QStrings;

{$DEFINE INTERFACE}	
const
	class_TInformation : TObjectClass = 'TInformation';

type
	PInformation = ^TInformation;
	TInformation = object(TObject)
	private
		{$IFDEF STRINGPTRS}
		PTypeID, PTitle, PFileName : PString;
		{$ELSE}
		FTypeID, FTitle, FFileName : String;
		{$ENDIF}
		{$IFDEF TEMPLATES}
		procedure CreatePMI; virtual;
		{$ENDIF}
	public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public
		constructor Create(AType : String); 
		destructor Destroy; virtual;
		function BaseName : String; virtual;
		function Path : String; virtual;
		function TypeID : String; virtual;
		function Title : String; virtual;
		procedure SetTitle (AValue : String); virtual;
		function FileName : String; virtual;
		procedure SetFileName (AValue : String); virtual;
	end;


{$UNDEF INTERFACE}

implementation

{$IFDEF TEMPLATES}
	uses QObjects, QDos;
{$ELSE}
	uses QDos;
	
	const
		ftESaver = 'ESAVER';
		ftProgram = 'PROGRAM';
		
		ESaverExt = '.SCR';
		
{$ENDIF}


{$DEFINE IMPLEMENTATION}
function TInformation.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TInformation) then 
		ObjectClass := class_TInformation
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TInformation.Create;
var
	ExecName : String;
begin
	ExecName := GetExeBase;
	if AType = '' then begin
		AType := 'UNKNOWN';
		ExecName := '';
	end;
	
	if AType = ftESaver then
		AddStr(ExecName, ESaverExt)
	else
	if AType = ftProgram then
		ExecName := GetExeName;
	
	{$IFDEF STRINGPTRS}
	PTypeID := StrPtr(AType);
	PTitle := StrPtr(BaseName);
	PFileName := StrPtr(ExecName);
	{$ELSE}
	FTypeID := AType;
	FTitle := BaseName;
	FFileName := ExecName
	{$ENDIF}
end;

destructor TInformation.Destroy;
begin
	{$IFDEF TEMPLATES}
		CreatePMI;
	{$ENDIF}
	
	{$IFDEF STRINGPTRS}
	FreeStr(PFileName);
	FreeStr(PTitle);
	FreeStr(PTypeID);
	{$ENDIF}
end;

{$IFDEF TEMPLATES}
procedure TInformation.CreatePMI;
var
	PMI : TConfigFile;
begin
	with PMI do begin
		Create('INFO');
		SetFileName(Path + BaseName + InfoExt);
		OpenSection('*');
		SetValue('TYPE', ftInfo);
		SetValue('CLASS', TypeID);
		SetValue('TITLE', Title);
		SetValue('FILE', FileName);
		Write;
		Destroy;
	end;
end;
{$ENDIF}

function TInformation.BaseName : String; 
begin
	BaseName := GetExeBase;
end;

function TInformation.Path : String; 
begin
	Path := Dir(GetExePath);
end;

function TInformation.TypeID : String; 
begin
	{$IFDEF STRINGPTRS}
		TypeID := PtrStr(PTypeID);
	{$ELSE}
		TypeID := FTypeID;
	{$ENDIF}
end;

function TInformation.Title : String; 
begin
	{$IFDEF STRINGPTRS}
		Title := PtrStr(PTitle);
	{$ELSE}
		Title := FTitle;
	{$ENDIF}
end;

procedure TInformation.SetTitle (AValue : String);
begin
	{$IFDEF STRINGPTRS}
		if Assigned(PTitle) then
			FreeStr(PTitle);
		PTitle := StrPtr(AValue);
	{$ELSE}
		FTitle := AValue;
	{$ENDIF}
end;

function TInformation.FileName : String; 
begin
	{$IFDEF STRINGPTRS}
		FileName := PtrStr(PFileName);
	{$ELSE}
		FileName := FFileName;
	{$ENDIF}
end;

procedure TInformation.SetFileName (AValue : String);
begin
	{$IFDEF STRINGPTRS}
		if Assigned(PFileName) then
			FreeStr(PFileName);
		PFileName := StrPtr(AValue);
	{$ELSE}
		FFileName := AValue;
	{$ENDIF}
end;



{$UNDEF IMPLEMENTATION}
end.
