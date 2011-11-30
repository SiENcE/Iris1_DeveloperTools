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

unit Unit_Models;

interface

uses File3DS, Const3DS, Types3DS, Utils3DS, Windows, Unit_OpenGL, Classes, Contnrs, Unit_ViewPort, Graphics,
Unit_Settings, Unit_OpenGLWrapper, Unit_OpenGLGeometry, SysUtils, Unit_Compatibility,
Unit_Matrix, Unit_TextureManager, DeCAL, Unit_Rasterer, Unit_LightNodes;

  type
    TSphere = record
       Center: TVector3f;
       Radius: Single;
    end;

    TModelFlag = (mfModelIsTileable, mfModelCastsShadows, mfModelHasParticleEffect);
    TModelFlags = set of TModelFlag;

    T3DSScaleMode = (smNone, smAuto, smCustom);

    TFaceFlag = (ffShowBackface);
    TFaceFlags = set of TFaceFlag;

    TLightSourceOptions = packed record
        FModelIsLightSource: Boolean;
        FFlickering: Boolean;

        FX, FY, FZ: Single;
        FRadius: Single;
        FR, FG, FB: Byte;

        FFlickerMinDelay: Integer;
        FFlickerMaxDelay: Integer;
        FFlickerAmount: Integer;

        Reserved: array[0..3] of Cardinal;
    end;

    TParticleEffectOptions = packed record
        EffectName: array[0..31] of Char;
        FX, FY, FZ: Single;
        FRotateEffect: LongBool;
        FAngle1: Single;
        FAngle2: Single;
        Reserved: array[0..5] of Cardinal;
    end;

    TFace = class;
    TNode = class (TObject)
         private
            FPoint: TVector3f;
            FSelected: Boolean;
            FFaces: TObjectList;
            FIndex: Integer; // Temp Value for Saving
            FNormal: TVector3f;
            procedure SetX (Value: Single);
            procedure SetY (Value: Single);
            procedure SetZ (Value: Single);
            function GetX: Single;
            function GetY: Single;
            function GetZ: Single;
            function GetFaceCount: Integer;
            function GetFace (Index: Integer): TFace;
         public
            property Point: TVector3f read FPoint write FPoint;
            property X: Single read GetX write SetX;
            property Y: Single read GetY write SetY;
            property Z: Single read GetZ write SetZ;
            property Selected: Boolean read FSelected write FSelected;
            property Index: Integer read FIndex write FIndex;
            property FaceCount: Integer read GetFaceCount;
            property Faces[Index: Integer]: TFace read GetFace;
            property Normal: TVector3f read FNormal;
            constructor Create (AX, AY, AZ: Single);
            destructor Destroy; override;
            function GetPositionID : Cardinal;
            procedure AddFace (AFace: TFace);
            procedure RemoveFace (AFace: TFace);
            procedure MergeNode (SourceNode: TNode);
            procedure CalcNormal;
            procedure CalcNormalForLightSurface (ALightSurface: Integer);
    end;

    TFace = class (TObject)
         private
            FNodes: array[0..2] of TNode;
            FNormal: TVector3f;
            FTexCoords: array [0..2, 0..1] of Single;
            FSelected: Boolean;
            FTexture: TTexture;
            FSurfaceIndex: Integer;
            FLightSurfaceIndex: Integer;
            FFlags: TFaceFlags;
            FLightNodeNormalIndices: array[0..2] of Integer;
            function GetNode (Index: Integer): TNode;
            function GetTexCoord (Index, UV: Integer): Single;
            procedure SetTexCoord (Index, UV: Integer; Value: Single);
         public
            property Nodes[Index: Integer]: TNode read GetNode;
            property Normal: TVector3f read FNormal;
            property Selected: Boolean read FSelected write FSelected;
            property Texture: TTexture read FTexture write FTexture;
            property TexCoords[Index, UV: Integer]: Single read GetTexCoord write SetTexCoord;
            property SurfaceIndex: Integer read FSurfaceIndex write FSurfaceIndex;
            property Flags: TFaceFlags read FFlags write FFlags;
            property LightSurfaceIndex: Integer read FLightSurfaceIndex write FLightSurfaceIndex;
            constructor Create (ANode1, ANode2, ANode3: TNode);
            destructor Destroy; override;
            procedure CalcNormal;
            procedure ChangeNode (OldNode, NewNode: TNode);
            procedure Invert;
            procedure FindSurface (ASurfaceIndex: Integer);
            function GetCommonNodes (AFace: TFace; UseTexCoords: Boolean): Integer;
            function HasCommonEdge (AFace: TFace; UseTexCoords: Boolean): Boolean;
            function IsEqualTo (AFace: TFace; UseTexCoords: Boolean): Boolean;
            procedure SetFlag (AFaceFlag: TFaceFlag);
            procedure ResetFlag (AFaceFlag: TFaceFlag);
            function GetFlag (AFaceFlag: TFaceFlag): Boolean;
            function CreateFileFlag: Cardinal;
            procedure SetFileFlag (AFileFlag: Cardinal);
    end;


    TModel = class (TObject)
           private
              FNodes: TObjectList;
              FFaces: TObjectList;
              FHeight: Integer;
              FSelectedNode: TNode;
              FSelectedFace: TFace;
              FModelID: Cardinal;
              FFlags: TModelFlags;
              FShadowIntensity: Integer;
              FLastChange: TDateTime;
              FLightSourceOptions: TLightSourceOptions;
              FEffectOptions: TParticleEffectOptions;

              function GetNode (Index: Integer): TNode;
              function GetNodeCount: Integer;
              function GetFace (Index: Integer): TFace;
              function GetFaceCount: Integer;
              function GetSelNodeCount: Integer;
              function GetSelFaceCount: Integer;

              function GetParticleEffectName: String;
              procedure SetParticleEffectName (Value: String);
           public
              property Nodes[Index: Integer]: TNode read GetNode;
              property NodeCount: Integer read GetNodeCount;
              property Faces[Index: Integer]: TFace read GetFace;
              property FaceCount: Integer read GetFaceCount;
              property SelectedNode: TNode read FSelectedNode;
              property SelectedFace: TFace read FSelectedFace;
              property Height: Integer read FHeight;
              property SelNodeCount: Integer read GetSelNodeCount;
              property SelFaceCount: Integer read GetSelFaceCount;
              property ModelID: Cardinal read FModelID;
              property Flags: TModelFlags read FFlags write FFlags;
              property ShadowIntensity: Integer read FShadowIntensity write FShadowIntensity;
              property LastChange: TDateTime read FLastChange write FLastChange;
              property ParticleEffectName: String read GetParticleEffectName write SetParticleEffectName;
              property LightSourceOptions: TLightSourceOptions read FLightSourceOptions write FLightSourceOptions;
              property EffectOptions: TParticleEffectOptions read FEffectOptions write FEffectOptions;
              constructor Create (AModelID: Cardinal);
              constructor CreateFromOldFormat (AModelID: Cardinal; Model: TStaticModel; ATexturesMap: DMap);
              constructor CreateFromStream (AStream: TStream);
              constructor CreateFrom3DS (AModelID: Cardinal; FileName: String; AScaleMode: T3DSScaleMode; AScaleFactor: Single; TranslateToDefaultPosition: Boolean);
              constructor CreateFromModel (AModel: TModel; AModelID: Cardinal);
              destructor Destroy; override;
              function AddNode (AX, AY, AZ: Single): TNode;
              function DupeNode (ANode: TNode): TNode;

              procedure RenderPoints;
              procedure RenderFaces (ATextureList: TGLInstanceTextureList);
              procedure RenderBackfaces;
              procedure RenderOutbox;
              procedure RenderGround;
              procedure RenderWireFrame;

              procedure SelectAllNodes (Select: Boolean);
              procedure InvertNodeSelection;
              procedure SelectAllFaces (Select: Boolean);
              procedure InvertFaceSelection;
              procedure FaceSelectionToPointSelection;
              procedure PointSelectionToFaceSelection;

              function MouseDown (AX, AY: Integer; AViewPort: TViewPort; Shift: TShiftState): Boolean;
              procedure CreateAFace;

              procedure FixErrors;
              function RemoveInvalidFaces: Integer; // Returns count of removed faces
              procedure InvertFaces;
              procedure FloatFaces;
              procedure DupeFaces;
              procedure DupeNodes;

              procedure RemoveFaces;
              procedure RemoveNodes;
              function RemoveFreeNodes: Integer;
              procedure RemoveFace(Face: TFace);

              procedure Save (AStream: TStream);
              procedure CreateGameFile (AStream: TStream; ASourceStream: TStream);

              function FindSurfaces: Integer;

              function GetBoundingSphere: TSphere;
              procedure CalcNormals;

              procedure SetEdgeLighted;
              procedure SetSmoothLighted;

              procedure CreateLightNodeList (AList: TLightNodeList);
              function CreateRaster (AResolution: Single): TRaster3D;

              procedure MirrorX;
              procedure MirrorY;
              procedure MirrorZ;
              procedure MirrorDiagonal;
              procedure Rotate90;
              procedure Rotate180;
              procedure Rotate270;

              procedure Scale (Factor: TVector3f);
              procedure Translate (Vector: TVector3f);

              // Apply Texture to surfaces (if OnlySelected is false, all surfaces are textured
              procedure ApplyTexture (ATexture: TTexture; OnlySelected: Boolean = True);
              procedure SelectFacesByTexture (ATexture: TTexture);
              procedure CalcFaceTextureCount (ATextureList: TGLInstanceTextureList);

              // Copys everything from "Model"
              procedure AssignFromModel (AModel: TModel);

              // Aligns all node to a Grid of size GridSize
              procedure AlignToGrid (GridSize: Single);

              // Counts TextureUsage
              procedure CountTextureUsage;

              // Exchanges Textures
              procedure ExchangeTexture (OldTexture, NewTexture: TTexture);

              // Returns, how many faces use "ATexture"
              function UsesTexture (ATexture: TTexture): Integer;

              // Returns the flags as String
              function GetFlagString: String;
           end;

    function GetGameModelLastChange (AStream: TStream): TDateTime;

implementation

const
   ModelFileSign: TFileSign = ('U', 'I', '3', 'M');
   GameModelFileSign: TFileSign = ('U', 'I', '3', 'G');

   MODELFILEFLAG_MODEL_IS_TILEABLE = 1;
   MODELFILEFLAG_MODEL_CASTS_SHADOWS = 2;
   MODELFILEFLAG_MODEL_HAS_PARTICLE_EFFECT = 4;

   FACEFILEFLAG_SHOW_BACKFACE = 1;

type
    TModelFileHeader = packed record
       Sign: TFileSign;
       Length: Cardinal;
       Version: Cardinal;
       ModelID: Cardinal;
       NodeStart: Cardinal;
       NodeCount: Cardinal;
       FaceStart: Cardinal;
       FaceCount: Cardinal;
       Flags: Cardinal;
       LightingStart: Cardinal;
       ShadowIntensity: Integer;
       LastChange: Double;
       EffectStart: Cardinal;
       Reserved: array[0..14] of Cardinal;
    end;

    TModelFileNode = packed record
       X, Y, Z: Single;
       FReserved: array[0..1] of Cardinal;
    end;

    TModelFileFace = packed record
       Nodes: array[0..2] of Cardinal;
       TextureMD5Sum: array [0..31] of Char;
       TexCoords: array[0..2, 0..1] of Single;
       FLightSurfaceIndex: Integer;
       FFlags: Cardinal;
       FReserved: array[0..2] of Cardinal;
    end;

    TGameModelFileHeader = packed record
       Sign: TFileSign;
       Length: Cardinal;
       Version: Cardinal;
       ModelID: Cardinal;
       Flags: Cardinal;
       NodeStart: Cardinal;
       NodeCount: Cardinal;
       FaceStart: Cardinal;
       FaceCount: Cardinal;
       FaceLightNodeStart: Cardinal;
       FaceLightNodeCount: Cardinal;
       PointLightNodeStart: Cardinal;
       PointLightNodeCount: Cardinal;
       ParallelLightNodeStart: Cardinal;
       ParallelLightNodeCount: Cardinal;
       LightSourceStart: Cardinal;
       BoundingSphere: array[0..3] of Single;
       RasterStart: Cardinal;
       LastChange: Double;
       EffectStart: Cardinal;
       Reserved: array[0..7] of Cardinal;
    end;

    TGameModelFileNode = packed record
       Vector: TVector3f;
       Normal: TVector3f;
    end;

    TGameModelParallelLightNode = packed record
       Normal: TVector3f;
    end;

    TGameModelPointLightNode = packed record
       Position: TVector3f;
       Normal: TVector3f;
    end;

    TGameModelFaceLightNode = packed record
       NodeIndex: Integer;
       PointLightNodeIndex: Integer;
       ParallelLightNodeIndex: Integer;
    end;

    TGameModelFileFace = packed record
       Nodes: array[0..2] of Cardinal;
       LightNodes: array[0..2] of Cardinal;
       Normal: array[0..2] of Single;
       TexCoords: array[0..2, 0..1] of Single;
       TextureIndex: Integer;
       Flags: Integer;
    end;


    function GetGameModelLastChange (AStream: TStream): TDateTime;
    var
       Header: TGameModelFileHeader;
    begin
        Assert (Assigned (AStream));
        AStream.Read (Header, sizeof (Header));
        result := Header.LastChange;
    end;

    function ConvertModelFlagsToInt (Flags: TModelFlags): Cardinal;
    begin
       result := 0;
       if mfModelIsTileable in Flags then
          result := result or MODELFILEFLAG_MODEL_IS_TILEABLE;
       if mfModelCastsShadows in Flags then
          result := result or MODELFILEFLAG_MODEL_CASTS_SHADOWS;
       if mfModelHasParticleEffect in Flags then
          result := result or MODELFILEFLAG_MODEL_HAS_PARTICLE_EFFECT;
    end;

    function ConvertIntToModelFlags (Flags: Cardinal): TModelFlags;
    begin
       result := [];
       if Flags and MODELFILEFLAG_MODEL_IS_TILEABLE <> 0 then
          result := result + [ mfModelIsTileable ];
       if Flags and MODELFILEFLAG_MODEL_CASTS_SHADOWS <> 0 then
          result := result + [ mfModelCastsShadows ];
       if Flags and MODELFILEFLAG_MODEL_HAS_PARTICLE_EFFECT <> 0 then
          result := result + [ mfModelHasParticleEffect ];
    end;

    constructor TNode.Create (AX, AY, AZ: Single);
    begin
         FFaces := TObjectList.Create(False);
         FPoint[0] := AX;
         FPoint[1] := AY;
         FPoint[2] := AZ;
    end;

    destructor TNode.Destroy;
    begin
         FFaces.Free;
         FFaces := nil;
    end;

    procedure TNode.AddFace (AFace: TFace);
    begin
         Assert(Assigned(AFace));
         FFaces.Add (AFace);
    end;

    procedure TNode.RemoveFace (AFace: TFace);
    var
      Index: Integer;
    begin
         For Index := FFaces.Count - 1 downto 0 do
             if FFaces[Index] = AFace then
                 FFaces.Delete(Index);
    end;

    procedure TNode.MergeNode (SourceNode: TNode);
    var
      Index: Integer;
    begin
         Assert(Assigned(SourceNode));
         For Index := 0 to SourceNode.FaceCount - 1 do begin
             SourceNode.Faces[Index].ChangeNode (SourceNode, Self);
             AddFace (SourceNode.Faces[Index]);
         end;
         SourceNode.FFaces.Clear;
    end;

    procedure TNode.SetX (Value: Single);
    begin
         FPoint[0] := Value;
    end;

    procedure TNode.SetY (Value: Single);
    begin
         FPoint[1] := Value;
    end;

    procedure TNode.SetZ (Value: Single);
    begin
         FPoint[2] := Value;
    end;

    function TNode.GetX: Single;
    begin
         result := FPoint[0];
    end;

    function TNode.GetY: Single;
    begin
         result := FPoint[1];
    end;

    function TNode.GetZ: Single;
    begin
         result := FPoint[2];
    end;


    function TNode.GetPositionID : Cardinal;
    var
      rx, ry, rz: Integer;
    begin
	rx := round ((Point[0] * 40.0) + 512.5);
        ry := round ((Point[1] * 40.0) + 512.5);
        rz := round ((Point[2] * 40.0) + 512.5);
	if (rx < 0) or (rx > 2047) or (ry < 0) or (ry > 2047) or (rz < 0) or (rz > 1023) then
           raise Exception.Create (Format('Points out of range : %d %d %d!', [rx, ry, rz]));

	result := (Cardinal (rx) shl 21) or (Cardinal(ry) shl 10) or Cardinal(rz);
    end;

    function TNode.GetFaceCount: Integer;
    begin
         result := FFaces.Count;
    end;

    function TNode.GetFace (Index: Integer): TFace;
    begin
         result := FFaces[Index] as TFace;
    end;

    procedure TNode.CalcNormal;
    var
      Index: Integer;
    begin
         FillChar (FNormal, sizeof(FNormal), 0);
         For Index := 0 to FaceCount - 1 do
             AddVector(FNormal, Faces[Index].Normal);
         if (FNormal[0] <> 0.0) or (FNormal[1] <> 0.0) or (FNormal[2] <> 0.0) then
            NormalizeVector (FNormal)
         else
            FNormal[2] := 1;
    end;

    procedure TNode.CalcNormalForLightSurface (ALightSurface: Integer);
    var
      Index: Integer;
    begin
         FillChar (FNormal, sizeof(FNormal), 0);
         For Index := 0 to FaceCount - 1 do
            if Faces[Index].FLightSurfaceIndex = ALightSurface then
                 AddVector(FNormal, Faces[Index].Normal);

         if (FNormal[0] <> 0.0) or (FNormal[1] <> 0.0) or (FNormal[2] <> 0.0) then
            NormalizeVector (FNormal)
         else
            FNormal[2] := 1;
    end;

    constructor TFace.Create (ANode1, ANode2, ANode3: TNode);
    var
      Index: Integer;
    begin
         inherited Create;
         Assert(Assigned(ANode1));
         Assert(Assigned(ANode2));
         Assert(Assigned(ANode3));
         FNodes[0] := ANode1;
         FNodes[1] := ANode2;
         FNodes[2] := ANode3;
         For Index := 0 to 2 do
             FNodes[Index].AddFace(Self);
         CalcNormal;
    end;

    destructor TFace.Destroy;
    var
      Index: Integer;
    begin
         For Index := 0 to 2 do
             if Assigned(FNodes[Index]) then
                  FNodes[Index].RemoveFace(Self);
    end;

    function TFace.GetNode (Index: Integer): TNode;
    begin
         if (Index < 0) or (Index > 2) then
            raise Exception.Create('node index out of range');
         result := FNodes[Index];
    end;

    procedure TFace.CalcNormal;
    var
      V1, V2: TVector3f;
    begin
      V1 := VectorSubtract (FNodes[1].Point,  FNodes[0].Point);
      V2 := VectorSubtract (FNodes[2].Point,  FNodes[0].Point);
      FNormal := VectorCrossProduct (V1, V2);
      FNormal[0] := - FNormal[0];
      FNormal[1] := - FNormal[1];
      FNormal[2] := - FNormal[2];
      NormalizeVector (FNormal);
    end;

    procedure TFace.ChangeNode (OldNode, NewNode: TNode);
    var
      Index: Integer;
    begin
      For Index := 0 to 2 do
         if FNodes[Index] = OldNode then
             FNodes[Index] := NewNode;
    end;

    procedure TFace.Invert;
    var
      TempNode: TNode;
      Index: Integer;
      TempTexCoords: array [0..2, 0..1] of Single;
    begin
      TempNode := FNodes[2];
      FNodes[2] := FNodes[0];
      FNodes[0] := TempNode;
      For Index := 0 to 2 do begin
          TempTexCoords[Index][0] := FTexCoords[Index][0];
          TempTexCoords[Index][1] := FTexCoords[Index][1];
      end;
      For Index := 0 to 2 do begin
          FTexCoords[Index][0] := TempTexCoords[2 - Index][0];
          FTexCoords[Index][1] := TempTexCoords[2 - Index][1];
      end;
      CalcNormal;
    end;

    function TFace.GetTexCoord (Index, UV: Integer): Single;
    begin
       Assert (Index >= 0);
       Assert (Index <= 2);
       Assert (UV >= 0);
       Assert (UV <= 1);
       result := FTexCoords[Index, UV];
    end;

    procedure TFace.SetTexCoord (Index, UV: Integer; Value: Single);
    begin
       Assert (Index >= 0);
       Assert (Index <= 2);
       Assert (UV >= 0);
       Assert (UV <= 1);
       FTexCoords[Index, UV] := Value;
    end;

    procedure TFace.FindSurface (ASurfaceIndex: Integer);
    var
       NodeIndex, FaceIndex: Integer;
       AFace: TFace;
    begin
       FSurfaceIndex := ASurfaceIndex;
       For NodeIndex := 0 to 2 do
           For FaceIndex := 0 to Nodes[NodeIndex].FaceCount - 1 do begin
               AFace := Nodes[NodeIndex].Faces[FaceIndex];
               if (AFace <> Self) and (AFace.Texture = Self.Texture) and (AFace.SurfaceIndex = 0) then
                  if HasCommonEdge (AFace, True) then
                     AFace.FindSurface (ASurfaceIndex);
           end;
    end;

    function TFace.GetCommonNodes (AFace: TFace; UseTexCoords: Boolean): Integer;
    var
      Index1, Index2: Integer;
    begin
      Assert(Assigned(AFace));
      Result := 0;
      For Index1 := 0 to 2 do
         For Index2 := 0 to 2 do
             if (FNodes[Index1] = AFace.FNodes[Index2]) and
                (not UseTexCoords or
                                  ((FTexCoords[Index1, 0] = AFace.FTexCoords[Index2, 0]) and
                                   (FTexCoords[Index1, 1] = AFace.FTexCoords[Index2, 1]))) then
                    inc (Result)
    end;

    function TFace.HasCommonEdge (AFace: TFace; UseTexCoords: Boolean): Boolean;
    begin
         result := GetCommonNodes (AFace, UseTexCoords) = 2;
    end;

    function TFace.IsEqualTo (AFace: TFace; UseTexCoords: Boolean): Boolean;
    begin
         result := GetCommonNodes (AFace, UseTexCoords) = 3;
    end;

    procedure TFace.SetFlag (AFaceFlag: TFaceFlag);
    begin
      FFlags := FFlags + [AFaceFlag ];
    end;

    procedure TFace.ResetFlag (AFaceFlag: TFaceFlag);
    begin
      FFlags := FFlags - [ AFaceFlag ];
    end;

    function TFace.GetFlag (AFaceFlag: TFaceFlag): Boolean;
    begin
      result := AFaceFlag in FFlags;
    end;

    function TFace.CreateFileFlag: Cardinal;
    begin
      result := 0;
      if ffShowBackface in FFlags then
        result := result + FACEFILEFLAG_SHOW_BACKFACE;
    end;

    procedure TFace.SetFileFlag (AFileFlag: Cardinal);
    begin
      FFlags := [];
      if AFileFlag and FACEFILEFLAG_SHOW_BACKFACE <> 0 then
          FFlags := FFlags + [ ffShowBackface ];
    end;

    constructor TModel.Create(AModelID: Cardinal);
    begin
       FNodes := TObjectList.Create;
       FFaces := TObjectList.Create;
       FHeight := 20;
       FModelID := AModelID;
       FFlags := [ mfModelCastsShadows ];
       FLightSourceOptions.FRadius := 5;
       FLightSourceOptions.FFlickerMinDelay := 100;
       FLightSourceOptions.FFlickerMaxDelay := 300;
       FLightSourceOptions.FFlickerAmount := 20;
    end;


    constructor TModel.CreateFromOldFormat (AModelID: Cardinal; Model: TStaticModel; ATexturesMap: DMap);
    var
      Index: Integer;
      ANodeList: array[0..3] of TNode;
      Face: TFace;
      Quad: PQuad;
      NodeCount, NodeIndex: Integer;
      Vertex: TQuadVertex;
      Iter: DIterator;
      Texture: TTexture;
    begin
       Create (AModelID);
       Assert(Assigned(Model));
       Assert(Assigned(ATexturesMap));

       FFlags := [ mfModelCastsShadows ];
       FLastChange := Now;

       if Model.FFlags and MODELFLAG_TILEABLE <> 0 then
          FFlags := FFlags + [mfModelIsTileable];

       For Index := 0 to Model.QuadCount - 1 do begin
           Quad := Model.GetQuad (Index);
           if Quad.Flags and QUADFLAG_TRIANGLE <> 0 then
              NodeCount := 3
           else
              NodeCount := 4;

           For NodeIndex := 0 to NodeCount - 1 do begin
               Vertex := Quad.Vertieces[NodeIndex];  // try to get the best out from a very ugly format ;)
               ANodeList[NodeIndex] := AddNode(Vertex.Vertex[2], 1 - Vertex.Vertex[0], Vertex.Vertex[1]);
           end;

           if Quad.Texture >= 0 then begin
               Iter := ATexturesMap.locate ( [ Quad.Texture ] );
               if not atEnd (Iter) then
                   Texture := getObject (Iter) as TTexture
               else
                   Texture := nil;
           end else
               Texture := pTextureManager.AddArtTexture(AModelID);

           Face := TFace.Create (ANodeList[0], ANodeList[1], ANodeList[2]);
           Face.Texture := Texture;
           Face.TexCoords [0, 0] := Quad.Vertieces[0].u;
           Face.TexCoords [0, 1] := 1.0 - Quad.Vertieces[0].v;
           Face.TexCoords [1, 0] := Quad.Vertieces[1].u;
           Face.TexCoords [1, 1] := 1.0 - Quad.Vertieces[1].v;
           Face.TexCoords [2, 0] := Quad.Vertieces[2].u;
           Face.TexCoords [2, 1] := 1.0 - Quad.Vertieces[2].v;
           if Quad.Flags and QUADFLAG_BACKFACE <> 0 then
               Face.SetFlag (ffShowBackface);
           FFaces.Add (Face);

           if NodeCount = 4 then begin
              Face := TFace.Create (ANodeList[2], ANodeList[3], ANodeList[0]);
              Face.Texture := Texture;
              Face.TexCoords [0, 0] := Quad.Vertieces[2].u;
              Face.TexCoords [0, 1] := 1.0 - Quad.Vertieces[2].v;
              Face.TexCoords [1, 0] := Quad.Vertieces[3].u;
              Face.TexCoords [1, 1] := 1.0 - Quad.Vertieces[3].v;
              Face.TexCoords [2, 0] := Quad.Vertieces[0].u;
              Face.TexCoords [2, 1] := 1.0 - Quad.Vertieces[0].v;
              FFaces.Add (Face);
              if Quad.Flags and QUADFLAG_BACKFACE <> 0 then
                  Face.SetFlag (ffShowBackface);
           end;
       end;

       FixErrors;
       RemoveInvalidFaces;
       if mfModelIsTileable in FFlags then
          SetEdgeLighted else
          SetSmoothLighted;
    end;

    constructor TModel.CreateFromStream (AStream: TStream);
    var
      Header: TModelFileHeader;
      Index, NodeIndex, TempIndex: Integer;
      ModelFileNode: TModelFileNode;
      Face: TFace;
      ModelFileFace: TModelFileFace;
      MD5Sum: String;
    begin
         Assert(Assigned(AStream));
         AStream.Read (Header, sizeof(Header));
         Assert (Header.Sign = ModelFileSign, 'invalid model file');
         if Header.Version > CURRENT_VERSION then
             raise Exception.Create ('This file needs a newer version of this editor');

         Create (Header.ModelID);

         if Header.LightingStart <> 0 then begin
              AStream.Position := Header.LightingStart;
              AStream.Read(FLightSourceOptions, sizeof(FLightSourceOptions));
         end;

         if Header.EffectStart <> 0 then begin
              AStream.Position := Header.EffectStart;
              AStream.Read(FEffectOptions, sizeof(FEffectOptions));
         end;

         FShadowIntensity := Header.ShadowIntensity;
         FLastChange := Header.LastChange;
         FFlags := ConvertIntToModelFlags (Header.Flags);

         AStream.Position := Header.NodeStart;
         For Index := 0 to Header.NodeCount - 1 do begin
             AStream.Read (ModelFileNode, sizeof (ModelFileNode));
             AddNode (ModelFileNode.X, ModelFileNode.Y, ModelFileNode.Z);
         end;

         AStream.Position := Header.FaceStart;
         For Index := 0 to Header.FaceCount - 1 do begin
             AStream.Read (ModelFileFace, sizeof (ModelFileFace));
             For NodeIndex := 0 to 2 do
                 Assert (ModelFileFace.Nodes[NodeIndex] < Cardinal(FNodes.Count), 'model file load error');
             Face := TFace.Create (Nodes [ModelFileFace.Nodes[0]],
                                   Nodes [ModelFileFace.Nodes[1]],
                                   Nodes [ModelFileFace.Nodes[2]]);

             For NodeIndex := 0 to 2 do begin
                 Face.TexCoords [NodeIndex, 0] := ModelFileFace.TexCoords[NodeIndex, 0];
                 Face.TexCoords [NodeIndex, 1] := ModelFileFace.TexCoords[NodeIndex, 1];
             end;

             Face.FLightSurfaceIndex := ModelFileFace.FLightSurfaceIndex;
             Face.SetFileFlag(ModelFileFace.FFlags);

             MD5Sum := '';
             For TempIndex := 0 to 31 do
                 MD5Sum := MD5Sum + ModelFileFace.TextureMD5Sum[TempIndex];

             if MD5Sum = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx' then
                 Face.Texture := pTextureManager.AddArtTexture (Header.ModelID)
             else
                 Face.Texture := pTextureManager.GetTexture (MD5Sum);
             FFaces.Add(Face);
         end;

    end;

    constructor TModel.CreateFrom3DS (AModelID: Cardinal; FileName: String; AScaleMode: T3DSScaleMode; AScaleFactor: Single; TranslateToDefaultPosition: Boolean);
    var
       DataFile: TFile3DS;
       Mesh: PMesh3DS;
       DataFace: TFace3DS;
       DataPoint: TPoint3DS;
       VertPos:TTexVert3DS;

       Face: TFace;
       Nodes: array[0..2] of TNode;

       Index, MeshIndex, I: Integer;
       V, MinP, MaxP: TVector3f;
       Size: Single;
       AMatrix, InvMatrix: TMMatrix;
    begin
        Create (AModelID);
        DataFile := TFile3DS.CreateFromFile(FileName);
        try
            FFlags := [ mfModelCastsShadows ];

            SetVector(MaxP, -100000, -100000, -100000);
            SetVector(MinP, 100000, 100000, 100000);
            for MeshIndex := 0 to DataFile.Objects.MeshCount - 1 do begin
                Mesh := DataFile.Objects.Mesh[MeshIndex];

                AMatrix := TMMatrix.Create;
                try
                    For I := 0 to 3 do begin
                        AMatrix[0, I] := Mesh.LocMatrix[I * 3];
                        AMatrix[1, I] := Mesh.LocMatrix[I * 3 + 1];
                        AMatrix[2, I] := Mesh.LocMatrix[I * 3 + 2];
                    end;

                    if TranslateToDefaultPosition then begin
                        InvMatrix := AMatrix.Inverse;
                    end else
                        InvMatrix := TMMatrix.Create;
                    try

                For Index := 0 to Mesh.NFaces - 1 do begin
                    DataFace := Mesh.FaceArray[Index];
                    For I := 0 to 2 do begin
                        DataPoint := Mesh.VertexArray[DataFace.FaceRec[I]];
                        V[0] := DataPoint.X;
                        V[1] := Datapoint.Y;
                        V[2] := DataPoint.Z;

                        InvMatrix.Apply (V[0], V[1], V[2]);

                        V [0] := - V[0];
                        V [1] := + V[1];
                        Nodes[I] := AddNode(V[0], V[1], V[2]);

                        if (V[0] > MaxP[0]) then MaxP[0] := V[0];
                        if (V[1] > MaxP[1]) then MaxP[1] := V[1];
                        if (V[2] > MaxP[2]) then MaxP[2] := V[2];
                        if (V[0] < MinP[0]) then MinP[0] := V[0];
                        if (V[1] < MinP[1]) then MinP[1] := V[1];
                        if (V[2] < MinP[2]) then MinP[2] := V[2];
                    end;

                    Face := TFace.Create(Nodes[0], Nodes[1], Nodes[2]);

                    if Mesh.NTextVerts > 0 then
                      For I := 0 to 2 do begin
                          VertPos := Mesh.TextArray[DataFace.FaceRec[I]];
                          Face.TexCoords[I, 0] := VertPos.U;
                          Face.TexCoords[I, 1] := 1 - VertPos.V;
                      end;
                    FFaces.Add(Face);
                end;

                    finally
                        InvMatrix.Free;
                    end;
                finally
                    AMatrix.Free;
                end;

            end;
        finally
           DataFile.Free;
        end;

        if NodeCount > 0 then begin
            V := VectorSubtract (MaxP, MinP);
{            if TranslateToDefaultPosition then
                Translate (VectorScale(MinP, -1.0)); // Move to (0/0/0)}

            if AScaleMode = smAuto then begin
                Size := V[0];
                if V[1] > Size then Size := V[1];
                if V[2] > Size then Size := V[2];
                if Size <> 0 then
                    Scale (AffineVectorMake(2 / Size, 2 / Size, 2 / Size)); // Resize Everything to a default size
            end;
            if AScaleMode = smCustom then
                Scale (AffineVectorMake (AScaleFactor / 20, AScaleFactor / 20, AScaleFactor / 20));
            if AScaleMode = smNone then
                Scale (AffineVectorMake (1 / 20, 1 / 20, 1 / 20));

            if TranslateToDefaultPosition then
                Translate (AffineVectorMake (0.5, 0.5, 0.0));

        end;


        FixErrors;
        RemoveInvalidFaces;
    end;

    constructor TModel.CreateFromModel (AModel: TModel; AModelID: Cardinal);
    begin
         Create (AModelID);
         AssignFromModel (AModel);
    end;

    destructor TModel.Destroy;
    begin
       FFaces.Free;
       FFaces := nil;
       FNodes.Free;
       FNodes := nil;
    end;

    procedure TModel.AssignFromModel (AModel: TModel);
    var
      Index, NodeIndex: Integer;
      Face, NewFace: TFace;
      TempNodes: array[0..2] of TNode;
    begin
         Assert(Assigned(AModel));

         FFaces.Clear;
         FNodes.Clear;

         FFlags := AModel.Flags;
         FShadowIntensity := AModel.ShadowIntensity;
         FLastChange := Now;
         FLightSourceOptions := AModel.FLightSourceOptions;

         For Index := 0 to AModel.FaceCount - 1 do begin
             Face := AModel.Faces[Index];
             For NodeIndex := 0 to 2 do
                 TempNodes[NodeIndex] := AddNode (Face.Nodes[NodeIndex].X, Face.Nodes[NodeIndex].Y, Face.Nodes[NodeIndex].Z);

             NewFace := TFace.Create (TempNodes [0], TempNodes [1], TempNodes [2]);

             For NodeIndex := 0 to 2 do begin
                 NewFace.TexCoords [NodeIndex, 0] := Face.TexCoords [NodeIndex, 0];
                 NewFace.TexCoords [NodeIndex, 1] := Face.TexCoords [NodeIndex, 1];
             end;

             NewFace.FLightSurfaceIndex := Face.FLightSurfaceIndex;
             NewFace.Flags := Face.Flags;
             NewFace.Texture := Face.Texture;
             FFaces.Add(NewFace);
         end;
    end;

    function TModel.GetNode (Index: Integer): TNode;
    begin
         result := FNodes[Index] as TNode;
    end;

    function TModel.GetNodeCount: Integer;
    begin
         result := FNodes.Count;
    end;

    function TModel.GetFace (Index: Integer): TFace;
    begin
         result := FFaces[Index] as TFace;
    end;

    function TModel.GetFaceCount: Integer;
    begin
         result := FFaces.Count;
    end;

    function TModel.AddNode (AX, AY, AZ: Single): TNode;
    begin
         result := TNode.Create(AX, AY, AZ);
         FNodes.Add(result);
    end;

    function TModel.DupeNode (ANode: TNode): TNode;
    begin
         Assert(Assigned(ANode));
         result := AddNode (ANode.X, ANode.Y, ANode.Z);
    end;

    procedure TModel.RenderPoints;
    var
      Index: Integer;
      Node: TNode;
    begin
       glDisable  (GL_LIGHTING);
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);
       glPointSize(5);

       glBegin (GL_POINTS);
       For Index := 0 to NodeCount - 1 do begin
          Node := Nodes[Index];
          if Node.Selected then
             SetColor (SelectedPointColor)
          else
             SetColor (PointColor);
          glVertex3fv (@Node.Point[0]);
       end;
       glEnd ();
       glPopMatrix ();

    end;

    procedure TModel.RenderFaces (ATextureList: TGLInstanceTextureList);
    var
      Index: Integer;
      Face: TFace;
    begin
       Assert (Assigned(ATextureList));
       ATextureList.Reset;

       glEnable  (GL_LIGHTING);
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);

       For Index := 0 to FaceCount - 1 do begin
          Face := Faces[Index];
          if Face.Selected then
             SetLightColor (clLime)
           else
             SetLightColor (FaceColor);

          if Assigned (Face.Texture) then
              glBindTexture( GL_TEXTURE_2D, ATextureList.GetGLTexture(Face.Texture))
          else
              glBindTexture( GL_TEXTURE_2D, 0);
          glBegin (GL_TRIANGLES);
          glNormal3fv (@Face.Normal[0]);
          glTexCoord2fv (@Face.FTexCoords[0, 0]);
          glVertex3fv (@Face.Nodes[0].Point[0]);
          glTexCoord2fv (@Face.FTexCoords[1, 0]);
          glVertex3fv (@Face.Nodes[1].Point[0]);
          glTexCoord2fv (@Face.FTexCoords[2, 0]);
          glVertex3fv (@Face.Nodes[2].Point[0]);
          glEnd ();
       end;
       glPopMatrix ();
    end;

    procedure TModel.RenderBackFaces;
    var
      Index: Integer;
      Face: TFace;
    begin
       glEnable  (GL_LIGHTING);
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);

       For Index := 0 to FaceCount - 1 do begin
          Face := Faces[Index];
          if Face.Selected then
             SetLightColor (clGreen)
           else begin
             if ffShowBackface in Face.Flags then
               SetLightColor (VisibleBackFaceColor)
             else
               SetLightColor (InVisibleBackFaceColor);
           end;
          glBegin (GL_TRIANGLES);
          glNormal3f (- Face.Normal[0], - Face.Normal[1], - Face.Normal[2]);
          glVertex3fv (@Face.Nodes[2].Point[0]);
          glVertex3fv (@Face.Nodes[1].Point[0]);
          glVertex3fv (@Face.Nodes[0].Point[0]);
          glEnd ();
       end;
       glPopMatrix ();
    end;

    procedure TModel.RenderWireFrame;
    var
      Index: Integer;
      Face: TFace;
    begin
       glDisable  (GL_LIGHTING);
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);

       SetColor (LineColor);

       For Index := 0 to FaceCount - 1 do begin
          Face := Faces[Index];
          glBegin (GL_LINE_STRIP);
          glNormal3fv (@Face.Normal[0]);
          glVertex3fv (@Face.Nodes[0].Point[0]);
          glVertex3fv (@Face.Nodes[1].Point[0]);
          glVertex3fv (@Face.Nodes[2].Point[0]);
          glVertex3fv (@Face.Nodes[0].Point[0]);
          glEnd ();
       end;
       glPopMatrix ();

    end;

    procedure TModel.RenderOutbox;
    begin
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);

       glDisable  (GL_LIGHTING);
       SetColor (LineColor);
       glBegin (GL_LINE_STRIP);
       glVertex3f (0, 0, 0);
       glVertex3f (0, 1, 0);
       glVertex3f (1, 1, 0);
       glVertex3f (1, 0, 0);
       glVertex3f (0, 0, 0);
       glVertex3f (0, 0, FHeight / 10);
       glVertex3f (0, 1, FHeight / 10);
       glVertex3f (1, 1, FHeight / 10);
       glVertex3f (1, 0, FHeight / 10);
       glVertex3f (0, 0, FHeight / 10);
       glEnd ();
       glBegin (GL_LINES);
       glVertex3f (0, 1, 0);
       glVertex3f (0, 1, FHeight / 10);
       glVertex3f (1, 1, 0);
       glVertex3f (1, 1, FHeight / 10);
       glVertex3f (1, 0, 0);
       glVertex3f (1, 0, FHeight / 10);
       glEnd ();

       glPopMatrix ();
    end;


    procedure TModel.RenderGround;
    begin
       glEnable  (GL_LIGHTING);
       glPushMatrix ();
       glTranslatef (-0.5, -0.5, -FHeight / 20);
       SetLightColor (GroundColor);
       glBegin (GL_QUADS);
       glNormal3f (0,0,1);
       glVertex3f (-2, -2, 0);
       glVertex3f (-2, +3, 0);
       glVertex3f (+3, +3, 0);
       glVertex3f (+3, -2, 0);
       glVertex3f (+3, -2, 0);
       glVertex3f (+3, +3, 0);
       glVertex3f (-2, +3, 0);
       glVertex3f (-2, -2, 0);
       glEnd ();
       glPopMatrix;
    end;

    function TModel.MouseDown (AX, AY: Integer; AViewPort: TViewPort; Shift: TShiftState): Boolean;
    var
      Index, NodeIndex: Integer;
      Node: TNode;
      Face: TFace;
      P: TPoint;
      Ray: TMRay;
      ALambda, MinLambda: Single;
      SelFace: TFace;
      Points: array[0..2] of TVector3f;
      vecTemp: TVector3f;
    begin
      result := False;
      For Index := 0 to NodeCount - 1 do begin
          Node := Nodes[Index];
          P := AViewPort.TransformCoord(Node.Point[0] - 0.5, Node.Point[1] - 0.5, Node.Point[2] - FHeight / 20);
          if (Abs (P.X - AX) < 5) and (Abs (P.Y - AY) < 5) then begin
             Node.Selected := not Node.Selected;
             result := True;
             if Node.Selected then begin
                FSelectedNode := Node;
                if not (ssShift in Shift) then
                   For NodeIndex := 0 to NodeCount - 1 do
                       if Nodes[NodeIndex] <> Node then
                          Nodes[NodeIndex].Selected := False;

             end else
                FSelectedNode := nil;
             break;
          end;
      end;

      if not Result then begin
         vecTemp [0] := 0.5;
         vecTemp [1] := 0.5;
         vecTemp [2] := FHeight / 20;
         Ray := AViewPort.GetPickRay (AX, AY);
         SelFace := nil;
         MinLambda := 1E15;
         try
            For Index := 0 to FaceCount - 1 do begin
                Face := Faces [Index];
                For NodeIndex := 0 to 2 do
                   Points [NodeIndex] := VectorSubtract (Face.Nodes[NodeIndex].Point, vecTemp);
                if Ray.InterSectWithTriangle (Points[0], Points[1], Points[2], ALambda) then
                   if ALambda < MinLambda then begin
                      SelFace := Face;
                      MinLambda := ALambda;
                   end;
            end;
         finally
            Ray.Free;
         end;

         if Assigned(SelFace) then begin
            SelFace.Selected := not SelFace.Selected;
            result := True;

            if SelFace.Selected then
               FSelectedFace := SelFace else
               FSelectedFace := nil;

            if not (ssShift in Shift) then
                For Index := 0 to FaceCount - 1 do
                    if Faces[Index] <> SelFace then
                        Faces[Index].Selected := False;

         end;

      end;

    end;


    function TModel.GetSelNodeCount: Integer;
    var
      Index: Integer;
    begin
      result := 0;
      For Index := 0 to NodeCount - 1 do
         if Nodes[Index].Selected then
            inc(result);
    end;

    function TModel.GetSelFaceCount: Integer;
    var
      Index: Integer;
    begin
      result := 0;
      For Index := 0 to FaceCount - 1 do
         if Faces[Index].Selected then
            inc(result);
    end;

    procedure TModel.CreateAFace;
    var
      Index: Integer;
      FSelNodes: array[0..2] of TNode;
      NodeIndex: Integer;
      Face: TFace;
    begin
      SelectAllFaces (False);
      NodeIndex := 0;
      For Index := 0 to NodeCount - 1 do
         if Nodes[Index].Selected then begin
            if NodeIndex > 2 then
                 raise Exception.Create('Face must contain 3 nodes');
            FSelNodes[NodeIndex] := Nodes[Index];
            inc (NodeIndex);
         end;

      if NodeIndex < 3 then
         raise Exception.Create('Face must contain 3 nodes');

      Face := TFace.Create(FSelNodes[0], FSelNodes[1], FSelNodes[2]);
      Face.Selected := True;
      FFaces.Add (Face);

    end;

    procedure TModel.SelectAllNodes (Select: Boolean);
    var
      Index: Integer;
    begin
       FSelectedNode := nil;
       For Index := 0 to NodeCount - 1 do
          Nodes[Index].Selected := Select;
    end;

    procedure TModel.InvertNodeSelection;
    var
      Index: Integer;
    begin
       FSelectedNode := nil;
       For Index := 0 to NodeCount - 1 do
          Nodes[Index].Selected := not Nodes[Index].Selected;
    end;

    procedure TModel.SelectAllFaces (Select: Boolean);
    var
      Index: Integer;
    begin
       FSelectedFace := nil;
       For Index := 0 to FaceCount - 1 do
          Faces[Index].Selected := Select;
    end;

    procedure TModel.InvertFaceSelection;
    var
      Index: Integer;
    begin
       FSelectedFace := nil;
       For Index := 0 to FaceCount - 1 do
          Faces[Index].Selected := not Faces[Index].Selected;
    end;

    procedure TModel.FixErrors; // Removes double nodes
    var
      Map: DMap;
      Iter: DIterator;
      Index: Integer;
      Node, OldNode: TNode;
      ID: Cardinal;
    begin
      Map := DMap.Create;
      try

         For Index := NodeCount - 1 downto 0 do begin
             Node := Nodes[Index];
             ID := Node.GetPositionID;
             Iter := Map.locate ([ID]);
             if not atEnd (Iter) then begin  // Node exists
                OldNode := DeCAL.getObject (Iter) as TNode;
                OldNode.MergeNode (Node);
                FNodes.Delete (Index);
             end else
                 Map.putPair([ID, Node]);
         end;

      finally
         Map.Free;
      end;

    end;

    function TModel.RemoveInvalidFaces: Integer; // Returns count of removed faces
    var
      Index: Integer;
      Face: TFace;
    begin
         result := 0;
         For Index := FaceCount - 1 downto 0 do begin
             Face := Faces[Index];
             if (Face.Nodes[0] = Face.Nodes[1]) or (Face.Nodes[0] = Face.Nodes[2]) or
                (Face.Nodes[1] = Face.Nodes[2]) then begin
                    FFaces.Delete (Index);
                    inc (result);
                end;
         end;
    end;

    procedure TModel.InvertFaces;
    var
       Index: Integer;
    begin
         For Index := 0 to FaceCount - 1 do
             if Faces[Index].Selected then
                  Faces[Index].Invert;
    end;

    procedure TModel.FloatFaces;
    var
       Face: TFace;
       Index, NodeIndex: Integer;
       OldNode, NewNode: TNode;
    begin
         For Index := 0 to FaceCount - 1 do
             if Faces[Index].Selected then begin
                Face := Faces[Index];
                For NodeIndex := 0 to 2 do
                  if Face.Nodes[NodeIndex].FaceCount <> 1 then begin
                     OldNode := Face.Nodes[NodeIndex];
                     OldNode.RemoveFace (Face);
                     NewNode := TNode.Create (OldNode.X, OldNode.Y, OldNode.Z);
                     Face.ChangeNode (OldNode, NewNode);
                     NewNode.AddFace (Face);
                     FNodes.Add(NewNode);
                  end;
             end;
             FaceSelectionToPointSelection;
    end;


    procedure TModel.FaceSelectionToPointSelection;
    var
      Index, FaceIndex: Integer;
      Node: TNode;
    begin
      For Index := 0 to NodeCount - 1 do begin
          Node := Nodes[Index];
          Node.Selected := False;
          For FaceIndex := 0 to Node.FaceCount - 1 do
               Node.Selected := Node.Selected or Node.Faces[FaceIndex].Selected;
      end;
    end;

    procedure TModel.PointSelectionToFaceSelection;
    var
      Index: Integer;
      Face: TFace;
    begin
      For Index := 0 to FaceCount - 1 do begin
          Face := Faces[Index];
          Face.Selected := Face.Nodes[0].Selected and Face.Nodes[1].Selected and Face.Nodes[2].Selected;
      end;
    end;


    procedure TModel.DupeNodes;
    var
       Index: Integer;
       Node: TNode;
    begin
       For Index := 0 to NodeCount - 1 do
           if Nodes[Index].Selected then begin
              Node := Nodes[Index];
              Node.Selected := False;
              Node := DupeNode (Node);
              Node.Selected := True;
           end;
       PointSelectionToFaceSelection;
    end;

    procedure TModel.DupeFaces;
    var
       Face, NewFace: TFace;
       ANodes: array[0..2] of TNode;
       Index, NodeIndex: Integer;
    begin
       For Index := 0 to FaceCount - 1 do
           if Faces[Index].Selected then begin
              Face := Faces[Index];
              For NodeIndex := 0 to 2 do
                  ANodes[NodeIndex] := DupeNode (Face.Nodes[NodeIndex]);
              Face.Selected := False;
              NewFace := TFace.Create (ANodes[0], ANodes[1], ANodes[2]);
              For NodeIndex := 0 to 2 do begin
                  NewFace.TexCoords[NodeIndex, 0] := Face.TexCoords[NodeIndex, 0];
                  NewFace.TexCoords[NodeIndex, 1] := Face.TexCoords[NodeIndex, 1];
              end;
              NewFace.Texture := Face.Texture;
              NewFace.FSurfaceIndex := Face.FSurfaceIndex;
              NewFace.FFlags := Face.Flags;
              FFaces.Add(NewFace);
              NewFace.Selected := True;
           end;
       FaceSelectionToPointSelection;
    end;

    procedure TModel.RemoveFaces;
    var
      Index: Integer;
    begin
       For Index := FaceCount - 1 downto 0 do
           if Faces[Index].Selected then
               FFaces.Delete(Index);
    end;

    procedure TModel.RemoveNodes;
    var
      Index, FaceIndex: Integer;
      Node: TNode;
    begin
       RemoveInvalidFaces;
       For Index := NodeCount - 1 downto 0 do
           if Nodes[Index].Selected then begin
               Node := Nodes[Index];
               For FaceIndex := Node.FaceCount - 1 downto 0 do
                   RemoveFace (Node.Faces[FaceIndex]);
               FNodes.Delete(Index);
           end;
    end;

    procedure TModel.RemoveFace(Face: TFace);
    var
      Index: Integer;
    begin
         For Index := FaceCount - 1 downto 0 do
            if FFaces[Index] = Face then
               FFaces.Delete(Index);
    end;


    function TModel.RemoveFreeNodes: Integer;
    var
      Index: Integer;
    begin
       result := 0;
       For Index := NodeCount - 1 downto 0 do
           if Nodes[Index].FaceCount = 0 then begin
               inc (result);
               FNodes.Delete (Index);
           end;
    end;

    procedure TModel.Save (AStream: TStream);
    var
      Header: TModelFileHeader;
      Position: Integer;
      Index, NodeIndex, TempIndex: Integer;
      Node: TNode;
      ModelFileNode: TModelFileNode;
      Face: TFace;
      ModelFileFace: TModelFileFace;
    begin
         Assert(Assigned(AStream));
         Position := AStream.Position;

         FillChar (Header, Sizeof (Header), 0);
         Header.Sign := ModelFileSign;
         Header.Version := 1;
         Header.ModelID := ModelID;
         Header.NodeCount := NodeCount;
         Header.FaceCount := FaceCount;
         Header.Flags := ConvertModelFlagsToInt (FFlags);
         Header.ShadowIntensity := FShadowIntensity;
         Header.LastChange := FLastChange;
         AStream.Write (Header, Sizeof (Header));

         Header.LightingStart := AStream.Position;
         AStream.Write (FLightSourceOptions, sizeof(FLightSourceOptions));

         Header.EffectStart := AStream.Position;
         AStream.Write (FEffectOptions, sizeof(FEffectOptions));

         Header.NodeStart := AStream.Position;
         For Index := 0 to NodeCount - 1 do begin
             Node := Nodes[Index];
             ModelFileNode.X := Node.X;
             ModelFileNode.Y := Node.Y;
             ModelFileNode.Z := Node.Z; // Adjust for other view space
             Node.Index := Index;
             AStream.Write (ModelFileNode, sizeof (ModelFileNode));
         end;

         Header.FaceStart := AStream.Position;
         For Index := 0 to FaceCount - 1 do begin
             Face := Faces[Index];
             FillChar (ModelFileFace, sizeof(ModelFileFace), 0);
             For NodeIndex := 0 to 2 do begin
                 ModelFileFace.Nodes[NodeIndex] := Face.Nodes[NodeIndex].Index;  // node index
                 ModelFileFace.TexCoords[NodeIndex, 0] := Face.TexCoords[NodeIndex, 0]; // u
                 ModelFileFace.TexCoords[NodeIndex, 1] := Face.TexCoords[NodeIndex, 1]; // v
                 ModelFileFace.FLightSurfaceIndex := Face.FLightSurfaceIndex;
             end;

             ModelFileFace.FFlags := Face.CreateFileFlag;

             // Write Texture MD5Sum
             if Assigned (Face.Texture) and (Face.Texture is TJPEGTexture)  then begin
                if Length (TJPEGTexture(Face.Texture).MD5Sum) = 32 then
                   For TempIndex := 0 to 31 do
                       ModelFileFace.TextureMD5Sum[TempIndex] := TJPEGTexture(Face.Texture).MD5Sum [TempIndex + 1];
             end;

             if Assigned (Face.Texture) and (Face.Texture is TArtTexture)  then
                   For TempIndex := 0 to 31 do
                       ModelFileFace.TextureMD5Sum[TempIndex] := 'x';

             AStream.Write (ModelFileFace, sizeof (ModelFileFace));
         end;

         Header.Length := AStream.Position - Position;
         AStream.Position := Position;
         AStream.Write (Header, Sizeof (Header));
         AStream.Position := Position + Integer(Header.Length);
    end;

    function TModel.FindSurfaces: Integer;
    var
      Index: Integer;
      ACounter: Integer;
    begin
       FixErrors;
       RemoveInvalidFaces;

       For Index := 0 to FaceCount - 1 do
            Faces[Index].SurfaceIndex := 0;

       ACounter := 0;
       For Index := 0 to FaceCount - 1 do
            if Faces[Index].SurfaceIndex = 0 then begin
               inc (ACounter);
               Faces[Index].FindSurface (ACounter);
            end;

       result := ACounter;

    end;

    procedure TModel.CreateGameFile (AStream: TStream; ASourceStream: TStream);
    var
      Header, SourceHeader: TGameModelFileHeader;
      Position: Integer;
      Index, NodeIndex, SourcePosition: Integer;
      Node: TNode;
      ModelFileNode: TGameModelFileNode;
      ModelFaceLightNode: TGameModelFaceLightNode;
      ModelPointLightNode: TGameModelPointLightNode;
      ModelParallelLightNode: TGameModelParallelLightNode;
      Face: TFace;
      ModelFileFace: TGameModelFileFace;
      Sphere: TSphere;

      ALightList: TLightNodeList;
      Raster, FuzzyRaster: TRaster3D;
    begin
         FixErrors;
         RemoveFreeNodes;
         CalcNormals;
         Assert(Assigned(AStream));
         Position := AStream.Position;

         FillChar (Header, Sizeof (Header), 0);
         Header.Sign := GameModelFileSign;
         Header.Version := CURRENT_VERSION;
         Header.ModelID := ModelID;
         Header.NodeCount := NodeCount;
         Header.FaceCount := FaceCount;
         Header.Flags := ConvertModelFlagsToInt(FFlags);
         Header.LastChange := FLastChange;
         Sphere := GetBoundingSphere;
         For Index := 0 to 2 do
            Header.BoundingSphere[Index] := Sphere.Center[Index];
         Header.BoundingSphere[3] := Sphere.Radius;
         AStream.Write (Header, Sizeof (Header));

         if (FLightSourceOptions.FModelIsLightSource) then begin // if no light source then file pointer is NULL
             Header.LightSourceStart := AStream.Position - Position;
             AStream.Write(FLightSourceOptions, sizeof(FLightSourceOptions));
         end;

         if (mfModelHasParticleEffect in Flags) then begin // if no light source then file pointer is NULL
             Header.EffectStart := AStream.Position - Position;
             AStream.Write(FEffectOptions, sizeof(FEffectOptions));
         end;

         Header.NodeStart := AStream.Position - Position;
         For Index := 0 to NodeCount - 1 do begin
             Node := Nodes[Index];
             ModelFileNode.Vector[0] := Node.X;
             ModelFileNode.Vector[1] := Node.Y;
             ModelFileNode.Vector[2] := Node.Z;
             ModelFileNode.Normal[0] := Node.Normal[0];
             ModelFileNode.Normal[1] := Node.Normal[1];
             ModelFileNode.Normal[2] := Node.Normal[2];
             Node.Index := Index; // set index before calculating light
             AStream.Write (ModelFileNode, sizeof (ModelFileNode));
         end;


         ALightList := TLightNodeList.Create;
         try
            // Precalculate lighting as far as possible
            CreateLightNodeList (ALightList);

            // Write FaceLightNodes
            Header.FaceLightNodeStart := AStream.Position - Position;
            Header.FaceLightNodeCount := ALightList.FaceLightNodeCount;
            For Index := 0 to ALightList.FaceLightNodeCount - 1 do begin
                ModelFaceLightNode.NodeIndex := ALightList.FaceLightNodes[Index].NodeIndex;
                ModelFaceLightNode.PointLightNodeIndex := ALightList.FaceLightNodes[Index].PointLightNodeIndex;
                ModelFaceLightNode.ParallelLightNodeIndex := ALightList.FaceLightNodes[Index].ParallelLightNodeIndex;
                AStream.Write (ModelFaceLightNode, sizeof(ModelFaceLightNode));
            end;

            // Write PointLightNodes
            Header.PointLightNodeStart := AStream.Position - Position;
            Header.PointLightNodeCount := ALightList.PointLightNodeCount;
            For Index := 0 to ALightList.PointLightNodeCount - 1 do begin
                ModelPointLightNode.Position := ALightList.PointLightNodes[Index].Position;
                ModelPointLightNode.Normal := ALightList.PointLightNodes[Index].Normal;
                AStream.Write (ModelPointLightNode, sizeof(ModelPointLightNode));
            end;

            // Write ParallelLightNodes
            Header.ParallelLightNodeStart := AStream.Position - Position;
            Header.ParallelLightNodeCount := ALightList.ParallelLightNodeCount;
            For Index := 0 to ALightList.ParallelLightNodeCount - 1 do begin
                ModelParallelLightNode.Normal := ALightList.ParallelLightNodes[Index].Normal;
                AStream.Write (ModelParallelLightNode, sizeof(ModelParallelLightNode));
            end;
         finally
            ALightList.Free;
         end;


         Header.FaceStart := AStream.Position - Position;
         For Index := 0 to FaceCount - 1 do begin
             Face := Faces[Index];
             FillChar (ModelFileFace, sizeof(ModelFileFace), 0);
             For NodeIndex := 0 to 2 do begin
                 ModelFileFace.Nodes[NodeIndex] := Face.Nodes[NodeIndex].Index;  // node index
                 ModelFileFace.LightNodes[NodeIndex] := Face.FLightNodeNormalIndices[NodeIndex];
                 ModelFileFace.TexCoords[NodeIndex, 0] := Face.TexCoords[NodeIndex, 0]; // u
                 ModelFileFace.TexCoords[NodeIndex, 1] := Face.TexCoords[NodeIndex, 1]; // v
             end;
             ModelFileFace.Normal[0] := Face.Normal[0];
             ModelFileFace.Normal[1] := Face.Normal[1];
             ModelFileFace.Normal[2] := Face.Normal[2];
             ModelFileFace.Flags := Face.CreateFileFlag;

             if Assigned (Face.Texture) then begin
                if Face.Texture is TJPEGTexture then
                   ModelFileFace.TextureIndex := TJPEGTexture(Face.Texture).Index
                else
                   ModelFileFace.TextureIndex := -1;
             end;

             AStream.Write (ModelFileFace, sizeof (ModelFileFace));
         end;

        if mfModelCastsShadows in FFlags then begin
             Header.RasterStart := AStream.Position - Position; // If no shadows, RasterStart = 0
             if not Assigned (ASourceStream) then begin
                   // Calculate Shadow Maps
                   Raster := CreateRaster(2.0 * 16);
                   try
                      FuzzyRaster := Raster.CreateFuzzyRaster(16, FShadowIntensity);
                   finally
                      Raster.Free;
                   end;

                   try
                      FuzzyRaster.SaveToStream(AStream);
                   finally
                      FuzzyRaster.Free;
                   end;
             end else begin
                 SourcePosition := ASourceStream.Position;
                 ASourceStream.Read(SourceHeader, sizeof (SourceHeader));
                 ASourceStream.Position := Integer(SourceHeader.RasterStart) + SourcePosition;
                 CopyRaster (AStream, ASourceStream);
             end;
         end;

         Header.Length := AStream.Position - Position;

{         SurfaceCount := FindSurfaces;
         For Index := 1 to SurfaceCount do begin
            Strips := CreateStrips(Index);
            Strips.Free;
         end;}

         AStream.Position := Position;
         AStream.Write (Header, Sizeof (Header));
         AStream.Position := Position + Integer(Header.Length);
    end;

    function TModel.GetBoundingSphere: TSphere;
    var
      NodeIndex: Integer;
      Sum, P: TVector3f;
      MaxDist, Dist: Single;
    begin
         FillChar (result, sizeof (Result), 0);
         if NodeCount > 0 then begin
              For NodeIndex := 0 to NodeCount - 1 do begin
                  P := Nodes[NodeIndex].Point;
                  Sum := VectorAdd (Sum, P);
              end;
              ScaleVector (Sum, 1.0 / NodeCount);
              MaxDist := 0;
              For NodeIndex := 0 to NodeCount - 1 do begin
                  P := Nodes[NodeIndex].Point;
                  Dist := VectorDistance (Sum, P);
                  if MaxDist < Dist then
                     MaxDist := Dist;
              end;
              result.Center := Sum;
              result.Radius := MaxDist;
         end;
    end;

    procedure TModel.CalcNormals;
    var
      Index: Integer;
    begin
         For Index := 0 to FaceCount - 1 do
             Faces[Index].CalcNormal;
         For Index := 0 to NodeCount - 1 do
             Nodes[Index].CalcNormal;
    end;

    procedure TModel.SetEdgeLighted;
    var
      Index: Integer;
    begin
       For Index := 0 to FaceCount - 1 do
          Faces[Index].FLightSurfaceIndex := Index;
    end;

    procedure TModel.SetSmoothLighted;
    var
      Index: Integer;
    begin
       For Index := 0 to FaceCount - 1 do
          Faces[Index].FLightSurfaceIndex := 0;
    end;

    procedure TModel.CreateLightNodeList (AList: TLightNodeList);
    var
      Index, LightSurface, NodeIndex: Integer;
      AMap: DMap;
      Iter: DIterator;
      AFace: TFace;
    begin
      Assert (Assigned (AList));
      AMap := DMap.Create;
      try
          For Index := 0 to FaceCount - 1 do begin
              Faces[Index].CalcNormal;
              AMap.putPair([Faces[Index].FLightSurfaceIndex, 0]);
          end;

          Iter := AMap.start;
          SetToKey (Iter);

          while not atEnd (Iter) do begin
              LightSurface := getInteger(Iter);

              For Index := 0 to NodeCount - 1 do
                  Nodes[Index].CalcNormalForLightSurface(LightSurface);

              For Index := 0 to FaceCount - 1 do
                 begin
                    AFace := Faces[Index];
                    if AFace.LightSurfaceIndex = LightSurface then
                        For NodeIndex := 0 to 2 do
                            AFace.FLightNodeNormalIndices[NodeIndex] := AList.AddFaceLightNode (AFace.Nodes[NodeIndex].FIndex, AFace.Nodes[NodeIndex].FPoint, AFace.Nodes[NodeIndex].Normal, mfModelIsTileable in FFlags).Index;
                end;

              Advance (Iter);
          end;

      finally
          AMap.Free;
      end;
    end;

    function TModel.CreateRaster (AResolution: Single): TRaster3D;
    var
      MinX, MinY, MinZ, MaxX, MaxY, MaxZ: Single;
      Index: Integer;
      Node: TNode;
      Face: TFace;
      V1, V2, V3: TVector3f;
    begin
        MinX := 0;
        MinY := 0;
        MinZ := 0;
        MaxX := 1;
        MaxY := 1;
        MaxZ := 1;
        For Index := 0 to NodeCount - 1 do begin
            Node := Nodes[Index];
            if MinX > Node.FPoint[0] then MinX := Node.FPoint[0];
            if MinY > Node.FPoint[1] then MinY := Node.FPoint[1];
            if MinZ > Node.FPoint[2] then MinZ := Node.FPoint[2];
            if MaxX < Node.FPoint[0] then MaxX := Node.FPoint[0];
            if MaxY < Node.FPoint[1] then MaxY := Node.FPoint[1];
            if MaxZ < Node.FPoint[2] then MaxZ := Node.FPoint[2];
        end;
        MinX := MinX - 1; // Umrandung lassen
        MinY := MinY - 1; // Umrandung lassen
        MinZ := MinZ - 1; // Umrandung lassen
        MaxX := MaxX + 1; // Umrandung lassen
        MaxY := MaxY + 1; // Umrandung lassen
        MaxZ := MaxZ + 1; // Umrandung lassen
        result := TRaster3D.Create(MinX * AResolution, MinY * AResolution, MinZ * AResolution,
                    MaxX * AResolution, MaxY * AResolution, MaxZ * AResolution);

        For Index := 0 to FaceCount - 1 do begin
            Face := Faces[Index];
            V1 := VectorScale(Face.FNodes[0].FPoint, AResolution);
            V2 := VectorScale(Face.FNodes[1].FPoint, AResolution);
            V3 := VectorScale(Face.FNodes[2].FPoint, AResolution);
            result.RenderTriangle (V1, V2, V3, VectorLength(VectorSubtract(V1, V2)), VectorLength(VectorSubtract(V2, V3)), VectorLength(VectorSubtract(V3, V1)));
        end;

    end;

    procedure TModel.MirrorX;
    var
      Index: Integer;
    begin
        For Index := 0 to NodeCount - 1 do
            Nodes[Index].FPoint[0] := 1.0 - Nodes[Index].FPoint[0];
        For Index := 0 to FaceCount - 1 do
            Faces[Index].Invert;
    end;

    procedure TModel.MirrorY;
    var
      Index: Integer;
    begin
        For Index := 0 to NodeCount - 1 do
            Nodes[Index].FPoint[1] := 1.0 - Nodes[Index].FPoint[1];
        For Index := 0 to FaceCount - 1 do
            Faces[Index].Invert;
    end;

    procedure TModel.MirrorZ;
    var
      Index: Integer;
    begin
        For Index := 0 to NodeCount - 1 do
            Nodes[Index].FPoint[2] := FHeight / 10 - Nodes[Index].FPoint[2];
        For Index := 0 to FaceCount - 1 do
            Faces[Index].Invert;
    end;

    procedure TModel.MirrorDiagonal;
    var
      Index: Integer;
      P: TVector3f;
    begin
        For Index := 0 to NodeCount - 1 do begin
            P := Nodes[Index].FPoint;
            Nodes[Index].FPoint[0] := P[1];
            Nodes[Index].FPoint[1] := P[0];
        end;

        For Index := 0 to FaceCount - 1 do
            Faces[Index].Invert;
    end;

    procedure TModel.Rotate90;
    begin
      MirrorDiagonal;
      MirrorX;
    end;

    procedure TModel.Rotate180;
    begin
      MirrorX;
      MirrorY;
    end;

    procedure TModel.Rotate270;
    begin
      MirrorDiagonal;
      MirrorY;
    end;

    procedure TModel.Scale (Factor: TVector3f);
    var
      Index: Integer;
    begin
        For Index := 0 to NodeCount - 1 do
            ScaleVector (Nodes[Index].FPoint, Factor);
        CalcNormals;
    end;

    procedure TModel.Translate (Vector: TVector3f);
    var
      Index: Integer;
    begin
        For Index := 0 to NodeCount - 1 do
            AddVector (Nodes[Index].FPoint, Vector);
    end;

    // Apply Texture to surfaces (if OnlySelected is false, all surfaces are textured
    procedure TModel.ApplyTexture (ATexture: TTexture; OnlySelected: Boolean = True);
    var
      Index: Integer;
    begin
      For Index := 0 to FaceCount - 1 do
          if (Faces[Index].Selected or not OnlySelected) then
              Faces[Index].FTexture := ATexture;
    end;

    procedure TModel.SelectFacesByTexture (ATexture: TTexture);
    var
      Index: Integer;
    begin
      For Index := 0 to FaceCount - 1 do
          if Faces[Index].Texture = ATexture then begin
             Faces[Index].Selected := True;
             FSelectedFace := Faces[Index];
          end;
    end;

    procedure TModel.CalcFaceTextureCount (ATextureList: TGLInstanceTextureList);
    var
      Index: Integer;
    begin
      Assert (Assigned (ATextureList));
      ATextureList.Reset; // Reset use counter
      For Index := 0 to FaceCount - 1 do
          if Assigned (Faces[Index].Texture) then
              ATextureList.GetGLTexture(Faces[Index].Texture); // Increase Use Count
    end;

    function TModel.GetParticleEffectName: String;
    var
     Index: Integer;
    begin
      result := '';
      For Index := 0 to 31 do
          if FEffectOptions.EffectName[Index] = #0 then
              break
          else
              result := result + FEffectOptions.EffectName[Index];
    end;

    procedure TModel.SetParticleEffectName (Value: String);
    var
      Len: Integer;
    begin
      Len := 32;
      if Len > Length (Value) then
          Len := Length (Value);
      FillChar (FEffectOptions.EffectName, sizeof (FEffectOptions.EffectName), 0);
      if Len > 0 then
          Move (PChar (Value)^, FEffectOptions.EffectName, Len);
    end;



    procedure TModel.AlignToGrid (GridSize: Single);
    var
       Index, Component: Integer;
    begin
       if GridSize < 0.00001 then
          raise Exception.Create ('Invalid Grid Size!');
       For Index := 0 to NodeCount - 1 do
          For Component := 0 to 2 do
              Nodes[Index].FPoint[Component] := round (Nodes[Index].FPoint[Component] / GridSize) * GridSize;

    end;

    procedure TModel.CountTextureUsage;
    var
        Index: Integer;
        AMap: DMap;
        Iter: DIterator;
        ATexture: TTexture;
    begin
        AMap := DMap.Create;
        try
            For Index := 0 to FaceCount - 1 do
                if Assigned (Faces[Index].Texture) then
                    AMap.putPair([Faces[Index].Texture, Faces[Index].Texture]);


            Iter := AMap.start;
            while not atEnd (Iter) do begin
                ATexture := getObject(Iter) as TTexture;
                ATexture.UseCount := ATexture.UseCount + 1;
                Advance (Iter);
            end;
        finally
            AMap.Free;
        end;
    end;

    procedure TModel.ExchangeTexture (OldTexture, NewTexture: TTexture);
    var
        Index: Integer;
    begin
        Assert (Assigned (OldTexture));
        Assert (Assigned (NewTexture));
        For Index := 0 to FaceCount - 1 do
            if Faces[Index].Texture = OldTexture then
                Faces[Index].FTexture := NewTexture;
    end;

    function TModel.UsesTexture (ATexture: TTexture): Integer;
    var
        Index: Integer;
    begin
        result := 0;
        For Index := 0 to FaceCount - 1 do
            if Faces[Index].Texture = ATexture then
                inc (result);
    end;

    function TModel.GetFlagString: String;
    begin
         result := '';
         if mfModelisTileable in Flags then  result := result + 'T';
         if mfModelCastsShadows in Flags then  result := result + 'S';
         if mfModelHasParticleEffect in Flags then  result := result + 'P';
         if FLightSourceOptions.FModelIsLightSource then
              result := result + 'L';
    end;

end.
