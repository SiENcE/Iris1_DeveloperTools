unit Unit_FastMesh;
//
// Created by: Alexander Oster - tensor@ultima-iris.de
//
(*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****)

interface

uses Classes, SysUtils, Unit_Matrix, Unit_OpenGLGeometry, Unit_OpenGL, Contnrs, ZLib, Dialogs,
Unit_Tools, Unit_Gts, Decal;

const

MESH_VERSION_1 = $01;

ARRAY_BIT_SIZE = 10;
ARRAY_COUNT = 1 shl ARRAY_BIT_SIZE;
ARRAY_AND_VALUE = ARRAY_COUNT - 1;

type

TPercentageCallback = procedure (Percentage: Integer; Caption: String = '') of Object;

PMeshFace = ^rMeshFace;

TMeshVector = array[0..2] of Single;
PMeshNode = ^rMeshNode;
rMeshNode = packed record
      FPosition: TMeshVector;
      FIndex: Integer;
end;
PNodeArray = ^TNodeArray;
TNodeArray = array[0..ARRAY_COUNT - 1] of rMeshNode;

PMeshEdgeFaceLink = ^rMeshEdgeFaceLink;
rMeshEdgeFaceLink = packed record
     Face: PMeshFace;
     Next: PMeshEdgeFaceLink;
end;

PMeshEdgeFaceLinkArray = ^TMeshEdgeFaceLinkArray;
TMeshEdgeFaceLinkArray = array[0..ARRAY_COUNT - 1] of rMeshEdgeFaceLink;

PMeshEdge = ^rMeshEdge;
rMeshEdge = packed record
      Nodes: array[0..1] of PMeshNode;
      FIndex: Integer;
      FFaces: PMeshEdgeFaceLink;
end;

PEdgeArray = ^TEdgeArray;
TEdgeArray = array[0..ARRAY_COUNT - 1] of rMeshEdge;

rMeshFace = packed record
     FEdges: array[0..2] of PMeshEdge;
     FFlag: Integer;
     FIndex: Integer;
end;
PFaceArray = ^TFaceArray;
TFaceArray = array[0..ARRAY_COUNT - 1] of rMeshFace;

procedure GetFaceNodes (AFace: PMeshFace; var Node1, Node2, Node3: PMeshNode);
function CalcFaceAngle (AFace1, AFace2: PMeshFace): Single;
function CalcFaceNormal (AFace: PMeshFace): TVector;
function GetFaceEdge (AFace: PMeshFace; ANode1, ANode2: PMeshNode): PMeshEdge;
function GetNewStripNode (OldFace, NewFace: PMeshFace): PMeshNode;
procedure GetFirstStripNodes (OldFace, NewFace: PMeshFace; Var Node1, Node2, Node3: PMeshNode);
procedure CalcFaceVolumeAndArea (AFace: PMeshFace; Var PyramidVolume, Area: Single);

type

TMesh = class (TObject)
    private
      FNodes: TList;
      FEdges: TList;
      FFaces: TList;
      FEdgeFaceLinks: TList;
      FNodeCount: Integer;
      FNodeCapacity: Integer;
      FEdgeCount: Integer;
      FEdgeCapacity: Integer;
      FFaceCount: Integer;
      FFaceCapacity: Integer;
      FEdgeFaceLinkCount: Integer;
      FEdgeFaceLinkCapacity: Integer;

      function GetNode (Index: Integer): PMeshNode;
      function GetEdge (Index: Integer): PMeshEdge;
      function GetFace (Index: Integer): PMeshFace;
      function GetEdgeFaceLink (Index: Integer): PMeshEdgeFaceLink;
      procedure LoadFromStream_Version1 (AStream: TStream);
      function AddNode: PMeshNode;
      function AddEdge: PMeshEdge;
      function AddFace: PMeshFace;
      function AddEdgeFaceLink: PMeshEdgeFaceLink;
      procedure AddFaceToEdge (AFace: PMeshFace; AEdge: PMeshEdge);
    public
      property NodeCount: Integer read FNodeCount;
      property Nodes[Index: Integer]: PMeshNode read GetNode;
      property EdgeCount: Integer read FEdgeCount;
      property Edges[Index: Integer]: PMeshEdge read GetEdge;
      property FaceCount: Integer read FFaceCount;
      property Faces[Index: Integer]: PMeshFace read GetFace;
      constructor Create;
      constructor CreateFromStream (AStream: TStream);
      constructor CreateFromFile (AFileName: String);
      destructor Destroy; override;
      procedure Save (AStream: TStream; AMatrix: TMMatrix);
      procedure SaveCompressed (AStream: TStream);
      function GetInterSectingFaceWithRay (ARay: TMRay; Var Lambda: Double): PMeshFace;
      procedure CalcVolumeAndArea (Var Volume, Area: Single);
      procedure ApplyMatrix (AMatrix: TMMatrix);
      procedure CreateFromMeshBuilder (AMeshBuilder: TObject);
//      function CreateStrips: TObjectList;
//      function CreateGTSSurface (AFaceMap: DMap): PGtsSurface;
end;




type

TMeshFileSign = array[0..3] of Char;

TMeshFileHeader = record // 384 Bytes
    Sign: TMeshFileSign;
    Version: Integer;
    NodeStart: Integer;
    NodeCount: Integer;
    EdgeStart: Integer;
    EdgeCount: Integer;
    FaceStart: Integer;
    FaceCount: Integer;
    Reserved: array[0..7] of Integer;
    STLName: array[0..63] of Char;
    Description: array[0..255] of Char;
end;

TCompressMeshFileHeader = packed record
    Sign: TMeshFileSign;
    Version: Integer;
    NodeStart: Integer;
    NodeCount: Integer;
    FaceStart: Integer;
    FaceCount: Integer;
    STLName: array[0..63] of Char;
end;

TMeshFileNode = record // 12 bytes
    X, Y, Z: Single;
end;

TMeshFileEdge = record  // 8 bytes
    Node1, Node2: Integer;
end;

TMeshFileFace = record  // 12 bytes
    Edge1, Edge2, Edge3: Integer;
end;

TSimpleNodeRec = packed record
  X, Y, Z: Integer;
end;

PMeshBuilderNode = ^TMeshBuilderNode;
PMeshBuilderEdge = ^TMeshBuilderEdge;
PMeshBuilderFace = ^TMeshBuilderFace;

PMeshBuilderNodeEdgeLink = ^TMeshBuilderNodeEdgeLink;
TMeshBuilderNodeEdgeLink = record
     Edge: PMeshBuilderEdge;
     Next: PMeshBuilderNodeEdgeLink;
end;

TMeshBuilderNode = packed record
    FPosition: TMeshVector;
    EdgeLinks: PMeshBuilderNodeEdgeLink;
    FIndex: Integer;
end;

TMeshBuilderFace = packed record
   Nodes: array[0..2] of PMeshBuilderNode;
   Edges: array[0..2] of PMeshBuilderEdge;
   FIndex: Integer;
end;

TMeshBuilderEdge = packed record
   FNodes: array[0..1] of PMeshBuilderNode;
   FIndex: Integer;
   FCount: Integer;
end;

PMeshBuilderNodeArray = ^TMeshBuilderNodeArray;
TMeshBuilderNodeArray = array[0..ARRAY_COUNT - 1] of TMeshBuilderNode;

PMeshBuilderNodeEdgeLinkArray = ^TMeshBuilderNodeEdgeLinkArray;
TMeshBuilderNodeEdgeLinkArray = array[0..ARRAY_COUNT - 1] of TMeshBuilderNodeEdgeLink;

PMeshBuilderFaceArray = ^TMeshBuilderFaceArray;
TMeshBuilderFaceArray = array[0..ARRAY_COUNT - 1] of TMeshBuilderFace;

PMeshBuilderEdgeArray = ^TMeshBuilderNodeArray;
TMeshBuilderEdgeArray = array[0..ARRAY_COUNT - 1] of TMeshBuilderNode;

TMeshBuilder = class (TObject)
   private
      FNodeCount, FNodeCapacity: Integer;
      FNodes: TList;
      FNodeEdgeLinkCount, FNodeEdgeLinkCapacity: Integer;
      FNodeEdgeLinks: TList;
      FFaceCount, FFaceCapacity: Integer;
      FFaces: TList;
      FEdgeCount, FEdgeCapacity: Integer;
      FEdges: TList;
   public
      constructor Create;
      destructor Destroy; override;
      function AddNode: PMeshBuilderNode;
      function AddFace: PMeshBuilderFace;
      function AddEdge: PMeshBuilderEdge;
      function AddNodeEdgeLink: PMeshBuilderNodeEdgeLink;
      function GetNode (Index: Integer): PMeshBuilderNode;
      function GetEdge (Index: Integer): PMeshBuilderEdge;
      function GetFace (Index: Integer): PMeshBuilderFace;
      function GetNodeEdgeLink (Index: Integer): PMeshBuilderNodeEdgeLink;
      procedure AddEdgeToNode (ANode: PMeshBuilderNode; AEdge: PMeshBuilderEdge);
      function CreateNewEdge (Node1, Node2: PMeshBuilderNode): PMeshBuilderEdge;
      procedure RemoveLastFace;
end;

implementation

const
  CompressedMeshFileSign: TMeshFileSign = ('t', 'G', 'c', 'm');

constructor TMeshBuilder.Create;
begin
      FNodes := TList.Create;
      FNodeEdgeLinks := TList.Create;
      FFaces := TList.Create;
      FEdges := TList.Create;
end;

destructor TMeshBuilder.Destroy;
var
  Index: Integer;
begin
      For Index := 0 to FNodes.Count - 1 do
          Dispose(FNodes[Index]);
      FNodes.Free;
      For Index := 0 to FFaces.Count - 1 do
          Dispose(FFaces[Index]);
      FFaces.Free;
      For Index := 0 to FEdges.Count - 1 do
          Dispose(FEdges[Index]);
      FEdges.Free;
      For Index := 0 to FFaces.Count - 1 do
          Dispose(FNodeEdgeLinks[Index]);
      FNodeEdgeLinks.Free;
end;

function TMeshBuilder.AddNode: PMeshBuilderNode;
var
   FArray: PMeshBuilderNodeArray;
begin
   if FNodeCount >= FNodeCapacity then begin
      New (FArray);
      FNodes.Add(FArray);
      inc (FNodeCapacity, ARRAY_COUNT);
   end;
   result := GetNode (FNodeCount);
   result.EdgeLinks := nil;
   inc (FNodeCount);
end;

function TMeshBuilder.GetNode(Index: Integer): PMeshBuilderNode;
begin
    result := @PMeshBuilderNodeArray(FNodes[Index shr ARRAY_BIT_SIZE])[Index and ARRAY_AND_VALUE];
end;

function TMeshBuilder.AddEdge: PMeshBuilderEdge;
var
   FArray: PMeshBuilderEdgeArray;
begin
   if FEdgeCount >= FEdgeCapacity then begin
      New (FArray);
      FEdges.Add(FArray);
      inc (FEdgeCapacity, ARRAY_COUNT);
   end;
   result := GetEdge (FEdgeCount);
   inc (FEdgeCount);
end;

function TMeshBuilder.GetEdge(Index: Integer): PMeshBuilderEdge;
begin
    result := @PMeshBuilderEdgeArray(FEdges[Index shr ARRAY_BIT_SIZE])[Index and ARRAY_AND_VALUE];
end;

function TMeshBuilder.AddNodeEdgeLink: PMeshBuilderNodeEdgeLink;
var
   FArray: PMeshBuilderNodeEdgeLinkArray;
begin
   if FNodeEdgeLinkCount >= FNodeEdgeLinkCapacity then begin
      New (FArray);
      FNodeEdgeLinks.Add(FArray);
      inc (FNodeEdgeLinkCapacity, ARRAY_COUNT);
   end;
   result := GetNodeEdgeLink (FNodeEdgeLinkCount);
   inc (FNodeEdgeLinkCount);
end;

function TMeshBuilder.GetNodeEdgeLink(Index: Integer): PMeshBuilderNodeEdgeLink;
begin
    result := @PMeshBuilderNodeEdgeLinkArray(FNodeEdgeLinks[Index shr ARRAY_BIT_SIZE])[Index and ARRAY_AND_VALUE];
end;

function TMeshBuilder.AddFace: PMeshBuilderFace;
var
   FArray: PMeshBuilderFaceArray;
begin
   if FFaceCount >= FFaceCapacity then begin
      New (FArray);
      FFaces.Add(FArray);
      inc (FFaceCapacity, ARRAY_COUNT);
   end;
   result := GetFace (FFaceCount);
   inc (FFaceCount);
end;

function TMeshBuilder.GetFace(Index: Integer): PMeshBuilderFace;
begin
    result := @PMeshBuilderFaceArray(FFaces[Index shr ARRAY_BIT_SIZE])[Index and ARRAY_AND_VALUE];
end;

procedure TMeshBuilder.RemoveLastFace;
begin
  if FFaceCount > 0 then
     dec (FFaceCount);
end;

procedure TMeshBuilder.AddEdgeToNode (ANode: PMeshBuilderNode; AEdge: PMeshBuilderEdge);
var
  ALink: PMeshBuilderNodeEdgeLink;
begin
  Assert (Assigned (ANode));
  ALink := AddNodeEdgeLink;
  ALink.Edge := AEdge;
  ALink.Next := ANode.EdgeLinks;
  ANode.EdgeLinks := ALink;
end;

function TMeshBuilder.CreateNewEdge (Node1, Node2: PMeshBuilderNode): PMeshBuilderEdge;
var
  Edge: PMeshBuilderEdge;
  ALink: PMeshBuilderNodeEdgeLink;
begin
  ALink := Node1.EdgeLinks;
  while Assigned (ALink) do begin
      Edge := ALink.Edge;
      Assert (Edge.FNodes[0] = Node1);
      if Edge.FNodes[1] = Node2 then begin
          result := Edge;
          inc (Edge.FCount);
          exit;
      end;
      ALink := ALink.Next;
  end;

  ALink := Node2.EdgeLinks;
  while Assigned (ALink) do begin
      Edge := ALink.Edge;
      Assert (Edge.FNodes[0] = Node2);
      if Edge.FNodes[1] = Node1 then begin
          result := Edge;
          inc (Edge.FCount);
          exit;
      end;
      ALink := ALink.Next;
  end;

  result := AddEdge;
  result.FNodes[0] := Node1;
  result.FNodes[1] := Node2;
  result.FIndex := -1;
  result.FCount := 0;

  AddEdgeToNode (Node1, result);

end;

procedure SaveNodeToStream (ANode: PMeshNode; AStream: TStream);
var
  Node: TMeshFileNode;
begin
  Node.X := ANode.FPosition[0];
  Node.Y := ANode.FPosition[1];
  Node.Z := ANode.FPosition[2];
  AStream.Write (Node, sizeof (Node));
end;

procedure SaveNodeToStreamWithMatrix (ANode: PMeshNode; AStream: TStream; AMatrix: TMMatrix);
var
  Node: TMeshFileNode;
begin
  Assert (Assigned (AMatrix));
  Node.X := ANode.FPosition[0];
  Node.Y := ANode.FPosition[1];
  Node.Z := ANode.FPosition[2];
  AMatrix.Apply(Node.X, Node.Y, Node.Z);
  AStream.Write (Node, sizeof (Node));
end;

procedure SaveEdgeToStream (AEdge: PMeshEdge; AStream: TStream);
var
  Edge: TMeshFileEdge;
begin
  Assert(Assigned(AStream));
  Edge.Node1 := AEdge.Nodes[0].FIndex;
  Edge.Node2 := AEdge.Nodes[1].FIndex;
  AStream.Write(Edge, sizeof(Edge));
end;

procedure SaveFaceToStream (AFace: PMeshFace; AStream: TStream);
var
  Face: TMeshFileFace;
begin
  Assert(Assigned(AStream));
  Face.Edge1 := AFace.FEdges[0].FIndex;
  Face.Edge2 := AFace.FEdges[1].FIndex;
  Face.Edge3 := AFace.FEdges[2].FIndex;
  AStream.Write(Face, sizeof(Face));
end;


procedure GetFaceNodes (AFace: PMeshFace; var Node1, Node2, Node3: PMeshNode);
begin
  Assert(Assigned(AFace));
  with AFace^ do begin
    if FEdges[0].Nodes[1] = FEdges[1].Nodes[0] then begin
        Node1 := FEdges[0].Nodes[0];
        Node2 := FEdges[0].Nodes[1];
        Node3 := FEdges[1].Nodes[1];
    end else
    if FEdges[0].Nodes[1] = FEdges[1].Nodes[1] then begin
        Node1 := FEdges[0].Nodes[0];
        Node2 := FEdges[0].Nodes[1];
        Node3 := FEdges[1].Nodes[0];
    end else
    if FEdges[0].Nodes[0] = FEdges[1].Nodes[0] then begin
        Node1 := FEdges[0].Nodes[1];
        Node2 := FEdges[0].Nodes[0];
        Node3 := FEdges[1].Nodes[1];
    end else begin
        Node1 := FEdges[0].Nodes[1];
        Node2 := FEdges[0].Nodes[0];
        Node3 := FEdges[1].Nodes[0];
    end;
  end;
end;

function CalcFaceAngle (AFace1, AFace2: PMeshFace): Single;
var
  Normal1, Normal2: TAffineVector;
  N1, N2: TVector;
  Index: Integer;
begin
   Assert(Assigned(AFace1));
   Assert(Assigned(AFace2));
   N1 := CalcFaceNormal(AFace1);
   N2 := CalcFaceNormal(AFace2);
   For Index := 0 to 2 do begin
      Normal1[Index] := N1[Index];
      Normal2[Index] := N2[Index];
   end;

   result := arccos( VectorAngle (Normal1, Normal2));
end;


function CalcFaceNormal (AFace: PMeshFace): TVector;
var
   Node1, Node2, Node3: PMeshNode;
   V1, V2: TVector;
   Index: Integer;
begin
   GetFaceNodes (AFace, Node1, Node2, Node3);
   For Index := 0 to 2 do begin
      V1[Index] := Node2.FPosition[Index] - Node1.FPosition[Index];
      V2[Index] := Node3.FPosition[Index] - Node1.FPosition[Index];
   end;
   result := VectorCrossProduct (V1, V2);
   NormalizeVector (result);
end;

function GetFaceEdge (AFace: PMeshFace; ANode1, ANode2: PMeshNode): PMeshEdge;
begin
  Assert (Assigned (AFace));
  with AFace^ do begin
    result := nil;
    if ((FEdges[0].Nodes[0] = ANode1) and (FEdges[0].Nodes[1] = ANode2)) or ((FEdges[0].Nodes[0] = ANode2) and (FEdges[0].Nodes[1] = ANode1)) then
      result := FEdges[0]
    else if ((FEdges[1].Nodes[0] = ANode1) and (FEdges[1].Nodes[1] = ANode2)) or ((FEdges[1].Nodes[0] = ANode2) and (FEdges[1].Nodes[1] = ANode1)) then
      result := FEdges[1]
    else if ((FEdges[2].Nodes[0] = ANode1) and (FEdges[2].Nodes[1] = ANode2)) or ((FEdges[2].Nodes[0] = ANode2) and (FEdges[2].Nodes[1] = ANode1)) then
      result := FEdges[2]
  end;
end;

procedure CalcFaceVolumeAndArea (AFace: PMeshFace; Var PyramidVolume, Area: Single);
var
     Node1, Node2, Node3: PMeshNode;
     V1, V2, V, U1, U2, U3: TVector;
     Index: Integer;
begin
      Assert (Assigned (AFace));
      GetFaceNodes (AFace, Node1, Node2, Node3);
      For Index := 0 to 2 do begin
         U1[Index] := Node1.FPosition[Index];
         U2[Index] := Node2.FPosition[Index];
         U3[Index] := Node3.FPosition[Index];
      end;

      V1 := VectorSubtract(U2, U1);
      V2 := VectorSubtract(U3, U1);
      V := VectorCrossProduct(V1, V2);
      Area := 0.5 * VectorLength(V);

      PyramidVolume := 1/6 * VectorDotProduct(U1,
                         VectorCrossProduct(U2, U3));
end;

constructor TMesh.Create;
begin
     FNodes := TList.Create;
     FEdges := TList.Create;
     FFaces := TList.Create;
     FEdgeFaceLinks := TList.Create;
end;

constructor TMesh.CreateFromStream (AStream: TStream);
begin
   Create;
   Assert(Assigned(AStream));
   LoadFromStream_Version1 (AStream);
end;

constructor TMesh.CreateFromFile (AFileName: String);
var
   Stream: TStream;
begin
   Stream := TFileStream.Create (AFileName, fmOpenRead);
   try
      CreateFromStream (Stream);
   finally
      Stream.Free;
   end;
end;

procedure TMesh.CreateFromMeshBuilder (AMeshBuilder: TObject);
   var
    FaceIndex: Integer;
    Index: Integer;
    MeshBuilder: TMeshBuilder;
    MeshFaceEdges: array[0..2] of PMeshEdge;
    MeshBuilderNode: PMeshBuilderNode;
    MeshBuilderEdge: PMeshBuilderEdge;
    MeshBuilderFace: PMeshBuilderFace;
    MeshNode: PMeshNode;
    MeshFace: PMeshFace;
begin
    MeshBuilder := AMeshBuilder as TMeshBuilder;
            For Index := 0 to MeshBuilder.FNodeCount - 1 do begin
                MeshBuilderNode := MeshBuilder.GetNode(Index);
                MeshBuilderNode.EdgeLinks := nil;
                MeshBuilderNode.FIndex := Index;
                MeshNode := AddNode;
                MeshNode.FPosition := MeshBuilderNode.FPosition;
                MeshNode.FIndex := Index;
            end;

            For FaceIndex := 0 to MeshBuilder.FFaceCount - 1 do begin
                MeshBuilderFace := MeshBuilder.GetFace(FaceIndex);
                For Index := 0 to 2 do begin
                    MeshBuilderFace.Edges[Index] := MeshBuilder.CreateNewEdge(MeshBuilderFace.Nodes[Index], MeshBuilderFace.Nodes[(Index + 1) mod 3]);
                end;
            end;

            for FaceIndex := 0 to MeshBuilder.FFaceCount - 1 do begin
                MeshBuilderFace := MeshBuilder.GetFace(FaceIndex);
                For Index := 0 to 2 do
                    if MeshBuilderFace.Edges[Index].FIndex = -1 then begin
                        MeshBuilderEdge := MeshBuilderFace.Edges[Index];
                        MeshBuilderEdge.FIndex := FEdgeCount;
                        MeshFaceEdges[Index] := AddEdge;
                        MeshFaceEdges[Index].Nodes[0] := GetNode(MeshBuilderEdge.FNodes[0].FIndex);
                        MeshFaceEdges[Index].Nodes[1] := GetNode(MeshBuilderEdge.FNodes[1].FIndex);
                        MeshFaceEdges[Index].FIndex := MeshBuilderEdge.FIndex;
                    end else
                        MeshFaceEdges[Index] := GetEdge (MeshBuilderFace.Edges[Index].FIndex);
                if not ((MeshFaceEdges[0] = MeshFaceEdges[1]) or (MeshFaceEdges[0] = MeshFaceEdges[1]) or (MeshFaceEdges[1] = MeshFaceEdges[2])) then begin
                    MeshFace := AddFace;
                    MeshFace.FIndex := MeshBuilderFace.FIndex;
                    For Index := 0 to 2 do begin
                        MeshFace.FEdges[Index] := MeshFaceEdges[Index];
                        AddFaceToEdge(MeshFace, MeshFaceEdges[Index]);
                    end;
                end;
            end;
end;


destructor TMesh.Destroy;
var
  Index: Integer;
begin
   For Index := 0 to FNodes.Count - 1 do
      Dispose(FNodes[Index]);
   FNodes.Free;

   For Index := 0 to FEdges.Count - 1 do
      Dispose(FEdges[Index]);
   FEdges.Free;

   For Index := 0 to FFaces.Count - 1 do
      Dispose(FFaces[Index]);
   FFaces.Free;

   For Index := 0 to FEdgeFaceLinks.Count - 1 do
      Dispose(FEdgeFaceLinks[Index]);
   FEdgeFaceLinks.Free;

   inherited Destroy;
end;

procedure TMesh.Save (AStream: TStream; AMatrix: TMMatrix);
var
  Header: TMeshFileHeader;
  Index: Integer;
begin
  Assert(Assigned(AStream));

  FillChar (Header, sizeof(Header), 0);
  Header.Sign[0] := 'M';
  Header.Sign[1] := 'E';
  Header.Sign[2] := 'S';
  Header.Sign[3] := 'H';
  Header.Version := MESH_VERSION_1;
  Header.NodeCount := FNodeCount;
  Header.EdgeCount := FEdgeCount;
  Header.FaceCount := FFaceCount;
  Header.NodeStart := sizeof (Header);
  Header.EdgeStart := Header.NodeStart + sizeof (TMeshFileNode) * FNodeCount;
  Header.FaceStart := Header.EdgeStart + sizeof (TMeshFileEdge) * FEdgeCount;
  AStream.Write(Header, sizeof(Header));
  if Assigned (AMatrix) then begin
     For Index := 0 to NodeCount - 1 do
         SaveNodeToStreamWithMatrix(GetNode(Index), AStream, AMatrix);
  end else begin
     For Index := 0 to NodeCount - 1 do
         SaveNodeToStream(GetNode(Index), AStream);
  end;
  For Index := 0 to EdgeCount - 1 do
     SaveEdgeToStream(GetEdge(Index), AStream);
  For Index := 0 to FaceCount - 1 do
     SaveFaceToStream(GetFace(Index), AStream);


end;

type
PFileNodeArray = ^TFileNodeArray;
TFileNodeArray = array[0..255] of TMeshFileNode;

PFileEdgeArray = ^TFileEdgeArray;
TFileEdgeArray = array[0..255] of TMeshFileEdge;

PFileFaceArray = ^TFileFaceArray;
TFileFaceArray = array[0..255] of TMeshFileFace;

procedure TMesh.LoadFromStream_Version1 (AStream: TStream);
var
  Header: TMeshFileHeader;
  Index, TempIndex: Integer;
  FileNodeArray: PFileNodeArray;
  FileEdgeArray: PFileEdgeArray;
  FileFaceArray: PFileFaceArray;
  FileNode: ^TMeshFileNode;
  FileEdge: ^TMeshFileEdge;
  FileFace: ^TMeshFileFace;
  ANode: PMeshNode;
  AEdge: PMeshEdge;
  AFace: PMeshFace;
begin
  Assert(Assigned(AStream));
  AStream.Read(Header, sizeof(Header));
  if (Header.Sign[0] <> 'M') or (Header.Sign[1] <> 'E') or
     (Header.Sign[2] <> 'S') or (Header.Sign[3] <> 'H') then
       raise Exception.Create('Invalid Mesh File');
  if Header.Version <> MESH_VERSION_1 then
     raise Exception.Create('Invalid Mesh Version');



  GetMem(FileNodeArray, sizeof (TMeshFileNode) * Header.NodeCount);
  try
     AStream.Position := Header.NodeStart;
     AStream.Read(FileNodeArray^, sizeof (TMeshFileNode) * Header.NodeCount);

     For Index := 0 to Header.NodeCount - 1 do begin
        ANode := AddNode;
        FileNode := @FileNodeArray[Index];
        ANode.FPosition[0] := FileNode^.X;
        ANode.FPosition[1] := FileNode^.Y;
        ANode.FPosition[2] := FileNode^.Z;
        ANode.FIndex := Index;
    end;
  finally
     FreeMem(FileNodeArray, sizeof (TMeshFileNode) * Header.NodeCount);
  end;

  GetMem(FileEdgeArray, sizeof (TMeshFileEdge) * Header.EdgeCount);
  try
      AStream.Position := Header.EdgeStart;
      AStream.Read(FileEdgeArray^, sizeof (TMeshFileEdge) * Header.EdgeCount);
      For Index := 0 to Header.EdgeCount - 1 do begin
          AEdge := AddEdge;
          FileEdge := @FileEdgeArray[Index];
          AEdge.Nodes[0] := Nodes[FileEdge.Node1];
          AEdge.Nodes[1] := Nodes[FileEdge.Node2];
          AEdge.FIndex := Index;
      end;
  finally
     FreeMem (FileEdgeArray);
  end;

  GetMem(FileFaceArray, sizeof (TMeshFileFace) * Header.FaceCount);
  try
       AStream.Position := Header.FaceStart;
       AStream.Read(FileFaceArray^, sizeof (TMeshFileFace) * Header.FaceCount);
       For Index := 0 to Header.FaceCount - 1 do begin
          AFace := AddFace;
          FileFace := @FileFaceArray[Index];
          AFace.FEdges[0] := Edges[FileFace.Edge1];
          AFace.FEdges[1] := Edges[FileFace.Edge2];
          AFace.FEdges[2] := Edges[FileFace.Edge3];
          For TempIndex := 0 to 2 do
              AddFaceToEdge(AFace, AFace.FEdges[TempIndex]);

       end;
   finally
      FreeMem(FileFaceArray);
   end;
end;


function TMesh.GetNode (Index: Integer): PMeshNode;
begin
     result := @PNodeArray(FNodes[Index shr ARRAY_BIT_SIZE])^[0];
     inc(result, Index and ARRAY_AND_VALUE);
end;


function TMesh.GetEdge (Index: Integer): PMeshEdge;
begin
     result := @PEdgeArray(FEdges[Index shr ARRAY_BIT_SIZE])^[0];
     inc(result, Index and ARRAY_AND_VALUE);
end;


function TMesh.GetFace (Index: Integer): PMeshFace;
begin
     result := @PFaceArray(FFaces[Index shr ARRAY_BIT_SIZE])^[0];
     inc(result, Index and ARRAY_AND_VALUE);
end;


function TMesh.GetEdgeFaceLink (Index: Integer): PMeshEdgeFaceLink;
begin
     result := @PMeshEdgeFaceLinkArray(FEdgeFaceLinks[Index shr ARRAY_BIT_SIZE])^[0];
     inc(result, Index and ARRAY_AND_VALUE);
end;


function TMesh.AddNode: PMeshNode;
var
  NodeArray: PNodeArray;
begin
     if FNodeCount >= FNodeCapacity then begin
        New (NodeArray);
        FillChar (NodeArray^, Sizeof(NodeArray^), 0);
        inc (FNodeCapacity, ARRAY_COUNT);
        FNodes.Add(NodeArray);
     end;
     result := GetNode(FNodeCount);
     inc (FNodeCount);
end;

function TMesh.AddEdge: PMeshEdge;
var
  EdgeArray: PEdgeArray;
begin
     if FEdgeCount >= FEdgeCapacity then begin
        New (EdgeArray);
        FillChar (EdgeArray^, Sizeof(EdgeArray^), 0);
        inc (FEdgeCapacity, ARRAY_COUNT);
        FEdges.Add(EdgeArray);
     end;
     result := GetEdge(FEdgeCount);
     inc (FEdgeCount);
end;

function TMesh.AddFace: PMeshFace;
var
  FaceArray: PFaceArray;
begin
     if FFaceCount >= FFaceCapacity then begin
        New (FaceArray);
        FillChar (FaceArray^, Sizeof(FaceArray^), 0);
        inc (FFaceCapacity, ARRAY_COUNT);
        FFaces.Add(FaceArray);
     end;
     result := GetFace(FFaceCount);
     inc (FFaceCount);
end;

function TMesh.AddEdgeFaceLink: PMeshEdgeFaceLink;
var
  EdgeFaceLinkArray: PMeshEdgeFaceLinkArray;
begin
     if FEdgeFaceLinkCount >= FEdgeFaceLinkCapacity then begin
        New (EdgeFaceLinkArray);
        FillChar (EdgeFaceLinkArray^, Sizeof(EdgeFaceLinkArray^), 0);
        inc (FEdgeFaceLinkCapacity, ARRAY_COUNT);
        FEdgeFaceLinks.Add(EdgeFaceLinkArray);
     end;
     result := GetEdgeFaceLink(FEdgeFaceLinkCount);
     inc (FEdgeFaceLinkCount);
end;

procedure TMesh.AddFaceToEdge (AFace: PMeshFace; AEdge: PMeshEdge);
var
  ALink: PMeshEdgeFaceLink;
begin
   Assert (Assigned (AFace));
   Assert (Assigned (AEdge));
   ALink := AddEdgeFaceLink;
   ALink.Face := AFace;
   ALink.Next := AEdge.FFaces;
   AEdge.FFaces := ALink;
end;



function TMesh.GetInterSectingFaceWithRay (ARay: TMRay; Var Lambda: Double): PMeshFace;
var
  FaceIndex, Index: Integer;
  ALambda: Single;
  Vectors: array[0..2] of TVector3f;
  Nodes: array[0..2] of PMeshNode;
begin
  result := nil;
  Lambda := 1E10;
  For FaceIndex := 0 to FaceCount - 1 do begin
      GetFaceNodes(GetFace(FaceIndex), Nodes[0], Nodes[1], Nodes[2]);
      For Index := 0 to 2 do begin
         Vectors[Index][0] := Nodes[Index].FPosition[0];
         Vectors[Index][1] := Nodes[Index].FPosition[1];
         Vectors[Index][2] := Nodes[Index].FPosition[2];
      end;
      if ARay.InterSectWithTriangle(Vectors[0], Vectors[1], Vectors[2], ALambda) then begin
          if ALambda < Lambda then begin
              Lambda := ALambda;
              result := Faces[FaceIndex];
          end;
      end;
  end;
end;



procedure TMesh.SaveCompressed (AStream: TStream);
var
  Index: Integer;
  Header: TCompressMeshFileHeader;
  ActRec, LastRec: TSimpleNodeRec;
  Node: PMeshNode;
  TempNodes: array[0..2] of PMeshNode;
  CompStream: TCompressionStream;
begin
  Assert (Assigned (AStream));
  FillChar (Header, Sizeof (Header), 0);
  Header.Sign := CompressedMeshFileSign;
  Header.Version := 1;
  Header.NodeStart := sizeof (Header);
  Header.NodeCount := NodeCount;
  Header.FaceStart := Header.NodeStart + NodeCount * 12;
  Header.FaceCount := FaceCount;

  CompStream := TCompressionStream.Create(clMax, AStream);
  try

     CompStream.Write(Header, sizeof(Header));
     FillChar (LastRec, Sizeof (LastRec), 0);
     For Index := 0 to NodeCount - 1 do begin
          Node := Nodes[Index];
          ActRec.X := round(Node.FPosition[0] * 100000);// - LastRec.X;
          ActRec.Y := round(Node.FPosition[1] * 100000);// - LastRec.Y;
          ActRec.Z := round(Node.FPosition[2] * 100000);// - LastRec.Z;
          LastRec := ActRec;
          CompStream.Write(ActRec, sizeof (ActRec));
     end;

     For Index := 0 to FaceCount - 1 do begin
         GetFaceNodes(Faces[Index], TempNodes[0],TempNodes[1],TempNodes[2]);
         ActRec.X := TempNodes[0].FIndex;
         ActRec.Y := TempNodes[1].FIndex;
         ActRec.Z := TempNodes[2].FIndex;
         CompStream.Write(ActRec, sizeof (ActRec));
     end;
  finally
     CompStream.Free;
  end;

end;

procedure TMesh.CalcVolumeAndArea (Var Volume, Area: Single);
var
  TempVolume, TempArea: Single;
  Index: Integer;
begin
  Volume := 0;
  Area := 0;
  For Index := 0 to FaceCount - 1 do begin
      CalcFaceVolumeAndArea(Faces[Index], TempVolume, TempArea);
      Volume := Volume + TempVolume;
      Area := Area + TempArea;
  end;

end;

procedure TMesh.ApplyMatrix (AMatrix: TMMatrix);
var
  Index: Integer;
  Node: PMeshNode;
begin
   Assert (Assigned (AMatrix));
   For Index := 0 to NodeCount - 1 do begin
        Node := Nodes[Index];
        AMatrix.Apply(Node.FPosition[0], Node.FPosition[1], Node.FPosition[2]);
   end;
end;

function GetNewStripNode (OldFace, NewFace: PMeshFace): PMeshNode;
var
  OldNodes: array[1..3] of PMeshNode;
  NewNodes: array[1..3] of PMeshNode;
begin
   result := nil;
   GetFaceNodes(OldFace, OldNodes[1], OldNodes[2], OldNodes[3]);
   GetFaceNodes(NewFace, NewNodes[1], NewNodes[2], NewNodes[3]);
   if (OldNodes[1] = NewNodes[2]) and (OldNodes[2] = NewNodes[1]) then
      result := NewNodes[3];
   if (OldNodes[1] = NewNodes[1]) and (OldNodes[2] = NewNodes[3]) then
      result := NewNodes[2];
   if (OldNodes[1] = NewNodes[3]) and (OldNodes[2] = NewNodes[2]) then
      result := NewNodes[1];

   if (OldNodes[2] = NewNodes[2]) and (OldNodes[3] = NewNodes[1]) then
      result := NewNodes[3];
   if (OldNodes[2] = NewNodes[1]) and (OldNodes[3] = NewNodes[3]) then
      result := NewNodes[2];
   if (OldNodes[2] = NewNodes[3]) and (OldNodes[3] = NewNodes[2]) then
      result := NewNodes[1];

   if (OldNodes[3] = NewNodes[2]) and (OldNodes[1] = NewNodes[1]) then
      result := NewNodes[3];
   if (OldNodes[3] = NewNodes[1]) and (OldNodes[1] = NewNodes[3]) then
      result := NewNodes[2];
   if (OldNodes[3] = NewNodes[3]) and (OldNodes[1] = NewNodes[2]) then
      result := NewNodes[1];

   Assert (Assigned (Result));

end;

procedure GetFirstStripNodes (OldFace, NewFace: PMeshFace; Var Node1, Node2, Node3: PMeshNode);
var
  OldNodes: array[1..3] of PMeshNode;
  NewNodes: array[1..3] of PMeshNode;
begin
   GetFaceNodes(OldFace, OldNodes[1], OldNodes[2], OldNodes[3]);
   GetFaceNodes(NewFace, NewNodes[1], NewNodes[2], NewNodes[3]);

   if    ((OldNodes[1] = NewNodes[2]) and (OldNodes[2] = NewNodes[1]))
      or ((OldNodes[1] = NewNodes[1]) and (OldNodes[2] = NewNodes[3]))
      or ((OldNodes[1] = NewNodes[3]) and (OldNodes[2] = NewNodes[2])) then begin
          Node1 := OldNodes[3];
          Node2 := OldNodes[1];
          Node3 := OldNodes[2];
   end else
   if    ((OldNodes[2] = NewNodes[2]) and (OldNodes[3] = NewNodes[1]))
      or ((OldNodes[2] = NewNodes[1]) and (OldNodes[3] = NewNodes[3]))
      or ((OldNodes[2] = NewNodes[3]) and (OldNodes[3] = NewNodes[2])) then begin
          Node1 := OldNodes[1];
          Node2 := OldNodes[2];
          Node3 := OldNodes[3];
   end else
   if    ((OldNodes[3] = NewNodes[2]) and (OldNodes[1] = NewNodes[1]))
      or ((OldNodes[3] = NewNodes[1]) and (OldNodes[1] = NewNodes[3]))
      or ((OldNodes[3] = NewNodes[3]) and (OldNodes[1] = NewNodes[2])) then begin
          Node1 := OldNodes[2];
          Node2 := OldNodes[3];
          Node3 := OldNodes[1];
   end else
      Assert (False);
end;
{
function TMesh.CreateStrips: TObjectList;
var
  GtsSurface: PGtsSurface;
  GtsStripList, GtsStrip, GtsFace: PGSList;
  Strip: TIntegerList;
  FMap: DMap;
  Iter: DIterator;
  LastMeshFace, MeshFace: PMeshFace;
  Node1, Node2, Node3: PMeshNode;
begin
  FMap := DMap.Create;
  try
     GtsSurface := CreateGTSSurface (FMap);
     try
         GtsStripList := gts_surface_strip (GtsSurface);
         result := TObjectList.Create;
         GtsStrip := GtsStripList;
         while Assigned (GtsStrip) do begin
             GtsFace := GtsStrip.Data;
             Strip := TIntegerList.Create;

             LastMeshFace := nil;
             while Assigned (GtsFace) do begin
                Iter := FMap.locate([GtsFace.Data]);
                Assert (not atEnd(Iter));
                MeshFace := getPointer (Iter);

                if Assigned (LastMeshFace) then begin
                    if Strip.Count = 0 then begin
                       GetFirstStripNodes (LastMeshFace, MeshFace, Node1, Node2, Node3);
                       Strip.Add(Node1.FIndex);
                       Strip.Add(Node2.FIndex);
                       Strip.Add(Node3.FIndex);
                    end else
                       Strip.Add(GetNewStripNode(LastMeshFace, MeshFace).FIndex);
                end;
                LastMeshFace := MeshFace;

                GtsFace := GtsFace.Next;

             end;
             result.Add(Strip);

             g_slist_free (GtsStrip.Data);
             GtsStrip := GtsStrip^.Next;
         end;

         g_slist_free (GtsStripList);
     finally
         gts_object_destroy (GtsSurface);
     end;
  finally
     FMap.Free;
  end;

end;


function TMesh.CreateGTSSurface(AFaceMap: DMap): PGtsSurface;
var
  Index: Integer;
  GtsVertieces, GtsEdges: TList;
  Node: PMeshNode;
  Edge: PMeshEdge;
  Face: PMeshFace;
  GtsFace: PGtsFace;
  GtsEdge: PGtsEdge;
begin
   result := gts_surface_new (gts_surface_class (), gts_face_class (), gts_edge_class (), gts_vertex_class ());
   GtsVertieces := TList.Create;
   try
      GtsVertieces.Capacity := NodeCount;
      For Index := 0 to NodeCount - 1 do begin
          Node := Nodes[Index];
          GtsVertieces.Add(gts_vertex_new (gts_vertex_class (), Node.FPosition[0], Node.FPosition[1], Node.FPosition[2]));
          Assert (Node.FIndex = Index);
      end;

      GtsEdges := TList.Create;
      try
         GtsEdges.Capacity := EdgeCount;
         For Index := 0 to EdgeCount - 1 do begin
             Edge := Edges[Index];
             Assert (Edge.Nodes[0].FIndex < NodeCount);
             Assert (Edge.Nodes[1].FIndex < NodeCount);
             GtsEdge := gts_edge_new (gts_edge_class (), GtsVertieces[Edge.Nodes[0].FIndex], GtsVertieces[Edge.Nodes[1].FIndex]);
             GtsEdges.Add(GtsEdge);
             Assert (Edge.FIndex = Index);
         end;

         For Index := 0 to FaceCount - 1 do begin
             Face := Faces[Index];
             Assert (Face.FEdges[0].FIndex < EdgeCount);
             Assert (Face.FEdges[1].FIndex < EdgeCount);
             Assert (Face.FEdges[2].FIndex < EdgeCount);
             GtsFace := gts_face_new (gts_face_class (), GtsEdges[Face.FEdges[0].FIndex], GtsEdges[Face.FEdges[1].FIndex], GtsEdges[Face.FEdges[2].FIndex]);
             gts_surface_add_face (result, GtsFace);
             if Assigned (AFaceMap) then
                AFaceMap.putPair([GtsFace, Face]);
         end;

      finally
         GtsEdges.Free;
      end;

   finally
      GtsVertieces.Free;
   end;


end;}


end.
