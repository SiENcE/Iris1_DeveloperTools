{==============================================================================

  Iris Model Editor - Copyright by Alexander Oster, tensor@ultima-iris.de

 The contents of this file are used with permission, subject to
 the Mozilla Public License Version 1.1 (the "License"); you may
 not use this file except in compliance with the License. You may
 obtain a copy of the License at
 http://www.mozilla.org/MPL/MPL-1.1.html

 Software distributed under the License is distributed on an
 "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 implied. See the License for the specific language governing
 rights and limitations under the License.

==============================================================================}

unit Unit_ModelLoader;

interface

uses Windows, Classes, DeCal, Unit_Models, Unit_Compatibility, Unit_TextureManager, SysUtils, MD5;

type
   TProgressCallback = procedure (Percentage: Integer; Caption: String = '') of Object;
   TModelLoader = class (TObject)
      private
         FMap: DMap;
         FLocks: DMap;
         function GetCount: Integer;
      public
         property Count: Integer read GetCount;
         constructor Create;
         destructor Destroy; override;
         procedure Import (AModelFile, ATextureDir: String; AProgressCallback: TProgressCallback);
         procedure Add (ID: Cardinal; AModel: TModel); // Needs the model to be unlocked
         procedure Remove (ID: Cardinal); // Needs the model to be unlocked
         function Get (ID: Cardinal): TModel;
         procedure Save (AStream: TStream); overload;
         procedure Save (AFileName: String); overload;
         procedure Load (AStream: TStream); overload;
         procedure Load (AFileName: String); overload;
         procedure CreateGameFile (AStream: TStream; AProgressCallback: TProgressCallback; APatchXML: TStrings); overload;
         procedure CreateGameFile (AFileName: String; AProgressCallback: TProgressCallback; APatchXML: TStrings); overload;
         procedure UpdateGameFile (AStream, ASourceStream: TStream; AProgressCallback: TProgressCallback; APatchXML: TStrings); overload;
         procedure UpdateGameFile (AFileName: String; AProgressCallback: TProgressCallback; APatchXML: TStrings); overload;
         procedure LockModel (ID: Cardinal);
         procedure UnLockModel (ID: Cardinal);
         procedure CountTextureUsage;
         procedure ExchangeTexture (OldTexture, NewTexture: TTexture);
   end;

   var
      pModelLoader: TModelLoader;
      pLightClipboard: TLightSourceOptions;
      pLightClipboardEmpty: Boolean = True;
      pModelClipboard: TModel = nil;

implementation

const
   ModelCollectionFileSign: TFileSign = ('U', 'I', '3', 'C');
   GameModelCollectionFileSign: TFileSign = ('U', 'I', '3', 'D');

type
   TModelCollectionHeader = packed record
       Sign: TFileSign;
       Length: Cardinal;
       Version: Cardinal;
       ModelTableStart: Cardinal;
       ModelTableCount: Cardinal;
       TextureStart: Cardinal;
       TextureLength: Cardinal;
       Reserved: array[0..8] of Cardinal;
   end;

   TModelTableEntry = packed record
       ID: Integer;
       Start: Integer;
       Length: Integer;
       MD5Sum: MD5Digest;
   end;

   PModelTableEntryArray = ^TModelTableEntryArray;
   TModelTableEntryArray = array[0..16383] of TModelTableEntry;



constructor TModelLoader.Create;
begin
   FMap := DMap.Create;
   FLocks := DMap.Create;
end;

destructor TModelLoader.Destroy;
begin
   ObjFree (FMap);
   FMap.Free;
   FMap := nil;
   FLocks.Free;
end;

procedure TModelLoader.Import (AModelFile, ATextureDir: String; AProgressCallback: TProgressCallback);
var
   StaticLoader: TStaticEngine;
   Index: Integer;
   AStaticModel: TStaticModel; // Old Format
   FMap: DMap;
begin
   if FLocks.size > 0 then
      raise Exception.Create ('Some models are edited at the moment..');

   StaticLoader := TStaticEngine.Create;
   try
       if Assigned (AProgressCallback) then
            AProgressCallback (0, 'Importing Data...');
       StaticLoader.Load (AModelFile, '');
       if Assigned (AProgressCallback) then
            AProgressCallback (0, 'Importing Textures...');
       FMap := pTextureManager.ImportTextures (StaticLoader.FTextureList, ATextureDir, AProgressCallback);
       try
          if Assigned (AProgressCallback) then
              AProgressCallback (0, 'Importing Statics...');

          For Index := 0 to 16383 do begin
              AStaticModel := StaticLoader.Get (Index);
              if Assigned (AStaticModel) then begin
                 Add (Index, TModel.CreateFromOldFormat (Index, AStaticModel, FMap));
              end;
              if Assigned (AProgressCallback) then
                  AProgressCallback(round (Index * 100 / 16384));
          end;
       finally
          FMap.Free;
       end;

   finally
       StaticLoader.Free;
   end;
end;

procedure TModelLoader.Add (ID: Cardinal; AModel: TModel);
var
   Iter: DIterator;
begin
   LockModel (ID);
   Assert(Assigned(AModel));
   Iter := FMap.locate ([ID]);
   if not atEnd (Iter) then
      getObject (Iter).Free;

   FMap.putPair ([ID, AModel]);
   UnLockModel (ID);
end;

function TModelLoader.Get (ID: Cardinal): TModel;
var
   Iter: DIterator;
begin
   Iter := FMap.locate ([ID]);
   if not atEnd (Iter) then
      result := getObject (Iter) as TModel
   else
      result := nil;
end;

function TModelLoader.GetCount: Integer;
begin
     result := FMap.size;
end;

procedure TModelLoader.Save (AStream: TStream);
var
   Header: TModelCollectionHeader;
   Position: Integer;
   Model: TModel;
   ModelTable: PModelTableEntryArray;
   Index: Integer;
   Iter: DIterator;
begin
     Assert (Assigned (AStream));
     FillChar(Header, sizeof (Header), 0);
     Header.Sign := ModelCollectionFileSign;
     Header.Version := CURRENT_VERSION;
     Header.ModelTableCount := FMap.size;
     Position := AStream.Position;

     AStream.Write (Header, sizeof(Header));

     Header.ModelTableStart := AStream.Position;

     Index := 0;
     GetMem(ModelTable, FMap.size * sizeof (TModelTableEntry));
     try
        FillChar (ModelTable^, FMap.size * sizeof (TModelTableEntry), 0);
        AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));

        Iter := FMap.start;
        while not atEnd (Iter) do begin
           Model := getObject (Iter) as TModel;
           ModelTable^[Index].Start := AStream.Position;
           Model.Save (AStream);
           ModelTable^[Index].Length := AStream.Position - ModelTable^[Index].Start;
           Advance (Iter);
           Inc (Index);
        end;

        AStream.Position := Header.ModelTableStart;
        AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));
        AStream.Position := AStream.Size;

        Header.TextureStart := AStream.Position;
        pTextureManager.Save (AStream);
        Header.TextureLength := AStream.Position - Integer(Header.TextureStart);

        AStream.Position := Position;
        AStream.Write (Header, sizeof(Header));
        AStream.Position := AStream.Size;

     finally
        FreeMem (ModelTable);
     end;
end;

procedure TModelLoader.Save (AFileName: String);
var
  Stream: TStream;
begin
   Stream := TFileStream.Create (AFileName, fmCreate);
   try
      Save (Stream);
   finally
      Stream.Free;
   end;
end;

procedure TModelLoader.Load (AStream: TStream);
var
   Header: TModelCollectionHeader;
   Model: TModel;
   ModelTable: PModelTableEntryArray;
   Index: Integer;
begin
     if FLocks.size > 0 then
       raise Exception.Create ('Some models are edited at the moment..');
     Assert (Assigned (AStream));
     AStream.Read (Header, sizeof (Header));
     Assert (Header.Sign = ModelCollectionFileSign, 'invalid file');
     if Header.Version > CURRENT_VERSION then
        raise Exception.Create ('This file needs a newer version of this editor');

     AStream.Position := Header.TextureStart;
     pTextureManager.Load (AStream);

     GetMem(ModelTable, Header.ModelTableCount * sizeof (TModelTableEntry));
     try
         AStream.Position := Header.ModelTableStart;
         AStream.Read (ModelTable^, Header.ModelTableCount * sizeof (TModelTableEntry));
         For Index := 0 to Header.ModelTableCount - 1 do begin
             AStream.Position := ModelTable^[Index].Start;
             Model := TModel.CreateFromStream (AStream);
            Add(Model.ModelID, Model);
         end;
     finally
        FreeMem (ModelTable);
     end;
end;

procedure TModelLoader.Load (AFileName: String);
var
  Stream: TStream;
begin
   Stream := TFileStream.Create (AFileName, fmOpenRead);
   try
      Load (Stream);
   finally
      Stream.Free;
   end;
end;

function CreateStreamDigest (AStream: TStream; Pos, Len: Integer): MD5Digest;
var
  OldPosition: Integer;
  Context: MD5Context;
  Data: Pointer;
begin
   Assert (Assigned (AStream));
   Assert (Len > 0);
   OldPosition := AStream.Position;
   GetMem(Data, Len);
   try
      AStream.Position := Pos;
      AStream.Read (Data^, Len);
      MD5Init (Context);
      MD5Update (Context, Data, Len);
      MD5Final (Context, result);
   finally
      FreeMem (Data);
   end;
   AStream.Position := OldPosition;
end;

procedure PatchXMLInit (APatchXML: TStrings);
begin
  if Assigned (APatchXML) then begin
      APatchXML.Clear;
      APatchXML.Add ('<?xml version="1.0"  encoding="ISO-8859-1" ?>');
      APatchXML.Add ('<patch>');
  end;
end;

procedure PatchXMLFinish (APatchXML: TStrings);
begin
  if Assigned (APatchXML) then begin
      APatchXML.Add ('</patch>');
      APatchXML.Add ('');
  end;
end;

procedure PatchXMLAddModel (APatchXML: TStrings; AEntry: TModelTableEntry);
begin
  if Assigned (APatchXML) then
      APatchXML.Add (Format('    <model id="%d" offset="%d" len="%d" md5sum="%s">', [AEntry.ID, AEntry.Start, AEntry.Length, MD5Print(AEntry.MD5Sum)]));
end;

procedure TModelLoader.CreateGameFile (AStream: TStream; AProgressCallback: TProgressCallback; APatchXML: TStrings);
var
   Header: TModelCollectionHeader;
   Position: Integer;
   Model: TModel;
   ModelTable: PModelTableEntryArray;
   Index: Integer;
   Iter: DIterator;
   Count: Integer;
begin
     Assert (Assigned (AStream));
     PatchXMLInit (APatchXML);
     FillChar(Header, sizeof (Header), 0);
     Header.Sign := GameModelCollectionFileSign;
     Header.Version := CURRENT_VERSION;
     Header.ModelTableCount := FMap.size;
     Position := AStream.Position;

     AStream.Write (Header, sizeof(Header));

     Header.ModelTableStart := AStream.Position;

     Index := 0;
     GetMem(ModelTable, FMap.size * sizeof (TModelTableEntry));
     try
        FillChar (ModelTable^, FMap.size * sizeof (TModelTableEntry), 0);
        AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));

        Iter := FMap.start;
        Count := FMap.Size;
        while not atEnd (Iter) do begin
           Model := getObject (Iter) as TModel;

           ModelTable^[Index].ID := Model.ModelID;
           ModelTable^[Index].Start := AStream.Position;
           Model.CreateGameFile (AStream, nil);
           ModelTable^[Index].Length := AStream.Position - ModelTable^[Index].Start;
           ModelTable^[Index].MD5Sum := CreateStreamDigest (AStream, ModelTable^[Index].Start, ModelTable^[Index].Length);
           Advance (Iter);

           PatchXMLAddModel (APatchXML, ModelTable^[Index]);

           if Assigned (AProgressCallback) then
                AProgressCallback (Index * 100 div Count, Format('Creating Game File (Model %d/%d)', [Index, Count]));

           Inc (Index);
        end;

        AStream.Position := Header.ModelTableStart;
        AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));
        AStream.Position := AStream.Size;

        Header.TextureStart := AStream.Position;
        pTextureManager.Save (AStream, APatchXML);
        Header.TextureLength := AStream.Position - Integer(Header.TextureStart);
        Header.Length := AStream.Size;

        AStream.Position := Position;
        AStream.Write (Header, sizeof(Header));
        AStream.Position := AStream.Size;

     finally
        FreeMem (ModelTable);
     end;

     PatchXMLFinish (APatchXML);
end;


procedure TModelLoader.CreateGameFile (AFileName: String; AProgressCallback: TProgressCallback; APatchXML: TStrings);
var
  Stream: TStream;
begin
   Stream := TFileStream.Create (AFileName, fmCreate);
   try
      CreateGameFile (Stream, AProgressCallback, APatchXML);
   finally
      Stream.Free;
   end;
end;

procedure TModelLoader.UpdateGameFile (AFileName: String; AProgressCallback: TProgressCallback; APatchXML: TStrings);
var
  Stream, TempStream: TStream;
  TempName: String;
begin
  TempName := AFileName;
  repeat
    TempName := TempName + 'x';
  until not FileExists(TempName);

  TempStream := TFileStream.Create (AFileName, fmOpenRead);
  try
     try

       Stream := TFileStream.Create (TempName, fmCreate);
       try
          UpdateGameFile (Stream, TempStream, AProgressCallback, APatchXML);
       finally
          Stream.Free;
       end;
   finally
      TempStream.Free;
   end;

          try
              DeleteFile (AFileName);
              RenameFile (TempName, AFileName);
          except
          end;

     finally
        if FileExists (TempName) then
           try
              DeleteFile (TempName);
           except
           end;
     end;
end;

procedure TModelLoader.UpdateGameFile (AStream, ASourceStream: TStream; AProgressCallback: TProgressCallback; APatchXML: TStrings);
var
   Header, SourceHeader: TModelCollectionHeader;
   Position: Integer;
   Model: TModel;
   ModelTable: PModelTableEntryArray;
   ModelTableEntry: TModelTableEntry;
   Index: Integer;
   Iter, VersionIter: DIterator;
   Count: Integer;
   VersionData: DMap;
   LastChange: TDateTime;
begin
     Assert (Assigned (AStream));
     Assert (Assigned (ASourceStream));

     PatchXMLInit (APatchXML);

     VersionData := DMap.Create;
     try
         // Retrieve old Versions
         ASourceStream.Read(SourceHeader, sizeof(SourceHeader));
         For Index := 0 to SourceHeader.ModelTableCount - 1 do begin
             ASourceStream.Position := Integer(SourceHeader.ModelTableStart) + Index * sizeof (TModelTableEntry);
             ASourceStream.Read(ModelTableEntry, sizeof(ModelTableEntry));
             ASourceStream.Position := ModelTableEntry.Start;
             LastChange := GetGameModelLastChange (ASourceStream);
             Model := Get (ModelTableEntry.ID);

             // if we can use the old model, save the source index
             if Assigned (Model) and (Model.LastChange = LastChange) then
                  VersionData.putPair([ModelTableEntry.ID, Index]);
         end;

         FillChar(Header, sizeof (Header), 0);
         Header.Sign := GameModelCollectionFileSign;
         Header.Version := CURRENT_VERSION;
         Header.ModelTableCount := FMap.size;
         Position := AStream.Position;

         AStream.Write (Header, sizeof(Header));

         Header.ModelTableStart := AStream.Position;

         Index := 0;
         GetMem(ModelTable, FMap.size * sizeof (TModelTableEntry));
         try
            FillChar (ModelTable^, FMap.size * sizeof (TModelTableEntry), 0);
            AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));

            Iter := FMap.start;
            Count := FMap.Size;
            while not atEnd (Iter) do begin
               Model := getObject (Iter) as TModel;

               ModelTable^[Index].ID := Model.ModelID;
               ModelTable^[Index].Start := AStream.Position;
               VersionIter := VersionData.locate([Model.ModelID]);
               if atEnd(VersionIter) then
                   Model.CreateGameFile (AStream, nil)  // if no old version available, create new one
               else begin
                   // else copy from old stream
                   ASourceStream.Position := Integer(SourceHeader.ModelTableStart) + getInteger(VersionIter) * sizeof (TModelTableEntry);
                   ASourceStream.Read(ModelTableEntry, sizeof(ModelTableEntry));
                   ASourceStream.Position := ModelTableEntry.Start;
                   Model.CreateGameFile (AStream, ASourceStream);  // Create new version, but use old shadow maps
               end;

               ModelTable^[Index].Length := AStream.Position - ModelTable^[Index].Start;
               ModelTable^[Index].MD5Sum := CreateStreamDigest (AStream, ModelTable^[Index].Start, ModelTable^[Index].Length);

               Advance (Iter);

               PatchXMLAddModel (APatchXML, ModelTable^[Index]);

               if Assigned (AProgressCallback) then
                    AProgressCallback (Index * 100 div Count, Format('Creating Game File (Model %d/%d)', [Index, Count]));

               Inc (Index);
            end;

            AStream.Position := Header.ModelTableStart;
            AStream.Write (ModelTable^, FMap.size * sizeof (TModelTableEntry));
            AStream.Position := AStream.Size;

            Header.TextureStart := AStream.Position;
            pTextureManager.Save (AStream, APatchXML);
            Header.TextureLength := AStream.Position - Integer(Header.TextureStart);
            Header.Length := AStream.Size;

            AStream.Position := Position;
            AStream.Write (Header, sizeof(Header));
            AStream.Position := AStream.Size;

         finally
            FreeMem (ModelTable);
         end;
     finally
        VersionData.Free;
     end;
     PatchXMLFinish (APatchXML);
end;

procedure TModelLoader.Remove (ID: Cardinal);
var
   Iter: DIterator;
begin
   LockModel(ID);
   Iter := FMap.locate ([ID]);
   if not atEnd (Iter) then begin
      getObject (Iter).Free;
      FMap.remove([ID]);
   end;
   UnlockModel(ID);
end;

procedure TModelLoader.LockModel (ID: Cardinal);
var
  Iter: DIterator;
begin
   Iter := FLocks.locate([ID]);
   if not atEnd (Iter) then
      raise Exception.Create('Model #' + IntToStr(ID) + ' is already edited');
   FLocks.putPair([ID, true]);
end;

procedure TModelLoader.UnLockModel (ID: Cardinal);
begin
    FLocks.remove([ID]);
end;

procedure TModelLoader.CountTextureUsage;
var
    AModel: TModel;
    Iter: DIterator;
begin
    pTextureManager.ResetUseCount;

    Iter := FMap.start;
    while not atEnd (Iter) do begin
        AModel := getObject (Iter) as TModel;
        AModel.CountTextureUsage;
        Advance (Iter);
    end;
end;

procedure TModelLoader.ExchangeTexture (OldTexture, NewTexture: TTexture);
var
    AModel: TModel;
    Iter: DIterator;
begin
    Iter := FMap.start;
    while not atEnd (Iter) do begin
        AModel := getObject (Iter) as TModel;
        AModel.ExchangeTexture (OldTexture, NewTexture);
        Advance (Iter);
    end;

    CountTextureUsage;
end;

initialization
   pModelLoader := TModelLoader.Create;

finalization
   pModelLoader.Free;
   pModelLoader := nil;

end.
