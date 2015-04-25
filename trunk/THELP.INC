{ Copyright 2015 Jerome Shidel }

(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

{ ---THelp --- }

{$IFDEF INTERFACE}
const
	class_THelp = 'THelp';
	
type
	PHelp = ^THelp;
	THelp = object(TWindow)
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public { protected }
		FList : TTextView;
		procedure SetCurrentBounds(AValue : TBounds); virtual;
		procedure AddHelpFrom(AControl : PCOntrol);
	public
		constructor Create(AParent : PControl; AName : String);
		destructor Destroy; virtual;
		
		procedure ShowHelp(AValue : PControl); virtual;
		procedure BuildHelp; virtual;
	end;
{$ENDIF}

{$IFDEF IMPLEMENTATION}
const
	HelpSection : PControl = nil;

procedure FormatHelpTitle (AVisual : PControl; var AValue : String); far;
var
	D : TFormatData;
	P : PControl;
begin
	D.ID := fmtString;
	D.StringValue := '';
	P := HelpSection;
	while Assigned(P) and (D.StringValue = '') do begin
		if P^.ClassOf(class_TApplication) then
			D.StringValue := Trim(PApplication(P)^.FInfo.Title)
		else if P^.ClassOf(class_TWindow) then 
			D.StringValue := Trim(PWindow(P)^.FTitle.GetCaption);
		P := PControl(P^.Parent);
	end;
	if D.StringValue = '' then D.StringValue := 'Program';
	AValue := FormatStr(AValue, D, 1);
end;

function THelp.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_THelp) then 
		ObjectClass := class_THelp
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor THelp.Create;
var
	B : TBounds;
	C : integer;
begin
	C := TextAttr;
	TextAttr := $07;
	inherited Create(AParent, AName);
	FTitle.SetCaption('0; Help on "%0" ');
	FTitle.SetTextAttr($74);
	FTitle.OnFormat := FormatHelpTitle;
	
	Bounds(1, 1, 60, Hi(ScreenMax) - 1,B);
	SetBounds(B);	
	
	FStyle := bxSingle;
	
	Bounds(Width - 15,Height - 3, 12, 1, B);
	SetAnchors(abTop or abBottom);

	FAutoEndModal := True;
	TextAttr := C or $08;
	FVisible := False;
	FList.Create(@Self, 'TEXT');
	FList.SetFreeOnDestroy(False);
	FList.SetAnchors(alClient);
	FList.SetScrollEdge(True);
	FList.Sorted := False;
	HelpSection := nil;
	TextAttr := C;
end;

destructor THelp.Destroy;
begin
	inherited Destroy;
end;

procedure THelp.SetCurrentBounds(AValue : TBounds);
begin
	if Assigned(Parent) then begin
		AValue.Left := (PControl(Parent)^.Width div 2) - (AValue.Width div 2) + 1;
		AValue.Top := (PControl(Parent)^.Height div 2) - (AValue.Height div 2) + 1;
	end else begin
		AValue.Left := (Lo(ScreenMax) div 2) - (AValue.Width div 2) + 1;
		AValue.Top := (Hi(ScreenMax)  div 2) - (AValue.Height div 2) + 1;
	end; 
	inherited SetCurrentBounds(AValue);
end;

procedure THelp.ShowHelp(AValue : PControl); 
begin
	HelpSection := AValue;
	if (Not Assigned(HelpSection)) or (HelpSection = @Self) then begin
		HelpSection := Application;
		Hide;
	end;
	StartDrawing;
	BuildHelp; 
	FinishDrawing;
	ShowModal;
	FList.ClearItems;
	HelpSection := nil;
end;

procedure THelp.AddHelpFrom(AControl : PControl);
var
	CommandMap : PCodeMap;
	CommandCount : word;
	L : PListItem;
	D : array[0..5] of TFormatData;
	P : PControl;
	S, T : String;
	W : integer;
begin
	GetCommandMap(CommandMap, CommandCount);
	L := AControl^.FHelpList.First;
	if Assigned(L) then FList.AddItem('');
	D[0].ID := fmtString;
	D[0].StringValue := AControl^.GetNameID;
	D[1].ID := fmtString;
	D[1].StringValue := AControl^.GetClassID;
	D[2].ID := fmtString;
	D[2].StringValue := Application^.GetCommandName(AControl^.FCommand);
	D[3].ID := fmtString;
	D[3].StringValue := Application^.GetCommandName(AControl^.FRespond);
	D[4].ID := fmtString;
	D[4].StringValue := Application^.GetKeyName(AControl^.FKeyValue);
	S := '';
	while Assigned(L) or (S <> '') do begin
		if Assigned(L) then
			T := FormatStr(L^.GetNameID, D, 4)
		else 
			T := '';
		if T = '' then begin
			if S <> '' then begin
				FList.AddItem(S);
				S := '';
			end;
		end;
		
		if S <> '' then 
			S := S + ' ' + T
		else
			S := T;
			
		if S = '' then FList.AddItem('');
		
		while Length(S) > FList.Width do begin
			W := FList.Width;
			while (W > 0) and (S[W] <> ' ') do dec(W);
			if W = 0 then W := FList.Width;
			T := Copy(S, 1, W);
			S := LTrim(Copy(S, Length(T) + 1, Length(S)));				
			FList.AddItem(T);
		end;

		if Assigned(L) then 
			L := L^.Next;
	end;
	P := PControl(AControl^.First);
	while assigned(P) do begin
		if P^.FShown then AddHelpFrom(P);
		P := PControl(P^.Next);
	end;
end;

procedure THelp.BuildHelp;
{var 
	I : integer; }
begin
	FList.ClearItems; 

	AddHelpFrom(HelpSection);
	
{	for I := 1 to 30 do
		FList.AddItem('Extra Line: ' + IntStr(I)); }
end;
{$ENDIF}
