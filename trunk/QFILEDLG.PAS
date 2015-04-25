{ Copyright 2015 Jerome Shidel }

(* 
		
	This project and related files are subject to either the terms
	specified in the included LICENSE.TXT file or the GNU GPLv2.0.
	
*)

unit QFileDlg; { QuickCrt File Dialog Boxes }
{$I QCRT.DEF}
interface

{$DEFINE INTERFACE}

uses 
   {$IFDEF DEVLOG} QLog, {$ENDIF} 
	QCrt, QObjects, QControls;
	
const
	class_TDirDropList = 'TDirDropList';
	class_TOpenDialog = 'TOpenDialog';
	class_TSaveDialog = 'TSaveDialog';
	
type
	PDirDropList = ^TDirDropList;
	TDirDropList = object(TDropList)
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public
		constructor Create(AParent : PControl; AName : String);
		destructor Destroy; virtual;
	end;

{$I TDIRLIST.INC}
{$I TPATHDIALOG.INC}
{$I TFILEDIALOG.INC}

	POpenDialog = ^TOpenDialog;
	TOpenDialog = object (TFileDialog)
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public
		constructor Create(AParent : PControl; AName : String);
		destructor Destroy; virtual;
	end;

	PSaveDialog = ^TSaveDialog;
	TSaveDialog = object (TFileDialog)
    public { protected }
		function  ObjectClass ( AName : String ) : String; virtual;
	public
		constructor Create(AParent : PControl; AName : String);
		destructor Destroy; virtual;
	end;

{$UNDEF INTERFACE}

implementation

{$DEFINE IMPLEMENTATION}

uses QErrors, QFiles, QFmtStr, QDos, QStrings;

{--- TDirDropList ---}
function TDirDropList.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TDirDropList) then 
		ObjectClass := class_TDirDropList
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TDirDropList.Create(AParent : PControl; AName : String);
var
	I : Integer;
	S : String;
begin
	inherited Create(AParent, AName);
	SetSorted(True);
	for I := 1 to 26 do begin
		if IsValidDrive(I) then begin
			if IsRemovable(I) or IsCDROM(I) or IsRemote(I) then begin		
			end else begin	
				GetDir(I, S);
				AddItem(Dir(S));
			end;
		end;
	end;
	GetDir(0, S);
	FEdit.SetCaption(Dir(S));
end;

destructor TDirDropList.Destroy; 
begin
	inherited Destroy;
end;

{$I TDIRLIST.INC}
{$I TPATHDIALOG.INC}
{$I TFILEDIALOG.INC}

{--- TLoadDialog --- }
function TOpenDialog.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TOpenDialog) then 
		ObjectClass := class_TOpenDialog
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TOpenDialog.Create;
var
	B : TBounds;
begin
	inherited Create(AParent, AName);
	SetTitle('Open File');

	FBOK.SetCaption('Open');

	FBMKDir.SetVisible(False);

	with FPath do begin
		B := FCBounds;
		Inc(B.Width, 14);
		B.Height := 10;
		SetBounds(B);
	end;

	with FDirs do begin
		B := FCBounds;
		Inc(B.Width, 14);
		SetBounds(B);
	end;

	SetHelpText('This is a standard "Open File" dialog box.');
	AddHelpText('');
	AddHelpText('You may use the dropdown path list to select one of the systems current');
	AddHelpText('active directories. Or, you may type in one for direct access. Also, ' );
	AddHelpText('recently entered paths are stored into the list for quick selection.');
	AddHelpText('');
	AddHelpText('The filter list can be used filter the directory listings by file types. ' );
	AddHelpText('They will be sorted in the order listed. Also, if a filename is entered manually' );
	AddHelpText('that contains wildcards, it will be added as a user filter.');
	AddHelpText('');
end;

destructor TOpenDialog.Destroy;
begin
	inherited Destroy;
end;

{--- TSaveDialog --- }
function TSaveDialog.ObjectClass(AName : String) : String; 
begin
	if (AName = '') or (AName = class_TSaveDialog) then 
		ObjectClass := class_TSaveDialog
	else
		ObjectClass := inherited ObjectClass(AName);
end;

constructor TSaveDialog.Create;
begin
	inherited Create(AParent, AName);
	SetTitle('Save File As');
	FMustExist := False;
	FBOK.SetCaption('Save As');

	SetHelpText('This is a standard "Save File As" dialog box.');
	AddHelpText('');
	AddHelpText('You may use the dropdown path list to select one of the systems current');
	AddHelpText('active directories. Or, you may type in one for direct access. Also, ' );
	AddHelpText('recently entered paths are stored into the list for quick selection.');
	AddHelpText('');
	AddHelpText('When a specified directory does not exist, the Make Dir button will ' );
	AddHelpText('become available. ' );
	AddHelpText('');
	AddHelpText('The filter list can be used filter the directory listings by file types. ' );
	AddHelpText('They will be sorted in the order listed. Also, if a filename is entered manually' );
	AddHelpText('that contains wildcards, it will be added as a user filter.');
	AddHelpText('');

end;

destructor TSaveDialog.Destroy;
begin
	inherited Destroy;
end;

{$UNDEF IMPLEMENTATION}

end.

 
