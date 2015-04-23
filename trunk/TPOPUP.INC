{ Copyright 2015 Jerome Shidel }

(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

{ ---TPopUp --- }

{$IFDEF INTERFACE}
const
	class_TPopUp = 'TPopUp';
	
type
	PPopUp = ^TPopUp;
	TPopUp = object(TWindow)
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public
		constructor Create(AParent : PControl; AName : String);
		destructor Destroy; virtual;
	end;
{$ENDIF}

{$IFDEF IMPLEMENTATION}
function TPopUp.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TPopUp) then 
		ObjectClass := class_TPopUp
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TPopUp.Create;
begin
	inherited Create(AParent, AName);
	FAllowOutside := True;
	SetBorder(True);
	SetStyle(bxSingle);
	FCloseBtn.SetVisible(False);
	SetShadow(True);
	FAutoEndModal := True;
	FUseCaption := False;
	FVisible := False;
end;

destructor TPopUp.Destroy;
begin
	inherited Destroy;
end;

{$ENDIF}
