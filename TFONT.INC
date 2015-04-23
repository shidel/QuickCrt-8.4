{ Copyright 2015 Jerome Shidel }

(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

{ --- TFont --- }

{$IFDEF INTERFACE}
const
	class_TFont = 'TFont';
	
type
	PFont = ^TFont;
	TFont = object(TPersistent)
	private 
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public { protected }
		FFontPtr : Pointer;
		FFontSize : integer;
		FFileName : String;
		FEnabled : boolean;
		FWidth : byte;
		FHeight : byte;
		FHeader : TListItem;
		procedure FreeFont;
		function LoadFromFile : boolean; virtual;
		function SaveToFile(AFileName : String) : boolean; virtual;
	public
		constructor Create(AName : String);
		destructor Destroy; virtual;
		function GetFileName : String; virtual;	
		procedure SetFileName ( AValue : String ) ; virtual;
		function GetEnabled : boolean; virtual;
		procedure SetEnabled(AValue : boolean); virtual;
		function GetHeight : integer; virtual;
		function GetWidth : integer; virtual;
		function GetHeader : PListItem; virtual;
	end;
{$ENDIF}

{$IFDEF IMPLEMENTATION}
function TFont.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TFont) then 
		ObjectClass := class_TFont
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TFont.Create(AName : String);
begin
	inherited Create(AName);
	FFontPtr := nil;
	FFontSize := 0;
	FFileName := '';
	FEnabled := false;
	FWidth := 0;
	FHeight := 0;
	FHeader.Create('TFontHeader');
	FHeader.SetFreeOnDestroy(False);
end;

destructor TFont.Destroy; 	
begin
	FreeFont;
	FHeader.Destroy;
	inherited Destroy;
end;

function TFont.LoadFromFile : boolean; 
const
	FileID : String = 'QCRT-FONT'#0;
var
	F : TFile;
	Success : boolean;
	S : String;
begin
	Success := false;
	LoadFromFile := Success;
	FreeFont;
	if FFileName = '' then exit;
	{$IFDEF DEVLOG_FONTLOAD}
	WriteLog('++ Loading font ' + FFileName);
	LogInc;
	{$ENDIF}
	F.Create;
	F.Assign(FFileName);
	F.Reset;
	Success := F.IOResult = 0;
	if Success then begin
		S := ChrStr(#0, Length(FileID));
		F.ReadRecord(S[1], Length(FileID));
		Success := (S = FileID) and (IOResult = 0);
	end;
	while Success and (S <> '') do begin
		if Success then begin
			S := F.GetString;
			Success := IOResult = 0;
			if Success then FHeader.Add(New(PListItem, Create(S)));
		end;	
	end;
	if Success then begin
		F.ReadByte(FWidth);
		Success := (IOResult = 0) and (FWidth = 8); { Must be 8x?? font}						
	end;	
	if Success then begin
		F.ReadByte(FHeight);
		Success := IOResult = 0;
	end;	
	if Success then begin
		FFontSize := F.FileSize - F.FilePos;
		Success := (IOResult = 0) and (MaxAvail > FFontSize);
	end;	
	if Success then begin
		GetMem(FFontPtr, FFontSize);
		F.ReadRecord(FFontPtr^, FFontSize);
		Success := IOResult = 0;
	end;	
	if Not Success then FreeFont;
	F.Destroy;
	{$IFDEF DEVLOG_FONTLOAD}
	LogDec;
	WriteLog('++ Loading font completed ' + BoolStr(Success));
	{$ENDIF}
	LoadFromFile := Success;
end;

function TFont.SaveToFile(AFileName : String) : boolean;
const
	FileID : String = 'QCRT-FONT'#0;
var
	F : TFile;
	Success : boolean;
	S : String;
	P : PlistItem;
begin
	Success := false;
	SaveToFile := Success;
	{$IFDEF DEVLOG_FONTLOAD}
	WriteLog('++ Saving font ' + AFileName);
	LogInc;
	{$ENDIF}
	F.Create;
	F.Assign(AFileName);
	F.Rewrite;
	Success := F.IOResult = 0;
	if Success then begin
		S := ChrStr(#0, Length(FileID));
		F.WriteRecord(S[1], Length(FileID));
		Success := (IOResult = 0);
	end;
	P := FHeader.First;
	while Success and (Assigned(P)) do begin
		if Success then begin
			S := P^.GetNameID;
			F.PutString(S);
			Success := IOResult = 0;
			P := P^.Next;
		end;	
	end;
	if Success then begin
		F.WriteByte(FWidth);
		Success := (IOResult = 0); { Must be 8x?? font}						
	end;	
	if Success then begin
		F.WriteByte(FHeight);
		Success := IOResult = 0;
	end;	
	if Success then begin
		F.WriteRecord(FFontPtr^, FFontSize);
		Success := IOResult = 0;
	end;	
	F.Destroy;
	{$IFDEF DEVLOG_FONTLOAD}
	LogDec;
	WriteLog('++ Saving font completed ' + BoolStr(Success));
	{$ENDIF}
	SaveToFile := Success;
end;

function TFont.GetFileName : String;
begin
	GetFileName := FFileName;
end;

procedure TFont.SetFileName ( AValue : String );
begin
	if AValue = FFileName then exit;
	FFileName := AValue;
	if not LoadFromFile then FFileName := '';
end;

procedure TFont.FreeFont;
begin
	if FEnabled then SetEnabled(False);
	if not Assigned(FFontPtr) then exit;
	FreeMem(FFontPtr, FFontSize);
	FFontPtr := nil;
	FHeader.Clear;
	FWidth := 0;
	FHeight := 0;
end;

function TFont.GetEnabled : boolean ;
begin
	GetEnabled := FEnabled;
end;

procedure TFont.SetEnabled(AValue :Boolean);
begin
	if FEnabled = AValue then exit;
	if AValue then begin
		if not Assigned(FFontPtr) then exit;
		
		SetUserFont(FHeight, FFontPtr);
		InitQCrt;
		
	end else begin
		TextMode(CO80);
	end;
	FEnabled := AValue;
end;

function TFont.GetHeight : integer;
begin
	GetHeight := FHeight;
end;

function TFont.GetWidth : integer;
begin
	GetWidth := FWidth;
end;

function TFont.GetHeader : PListItem;
begin
	GetHeader := @FHeader;
end;
{$ENDIF}
