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

unit Unit_LightNodes;

interface

uses Unit_OpenGLGeometry, DeCal, Contnrs, SysUtils;
// This unit is used for creating the light information


type
    // contains a normal. this is used for parallel lighting, because you don't need any position
    TParallelLightNode = class (TObject)
        private
           FNormal: TVector3f;
           FIndex: Integer;
        public
           property Normal: TVector3f read FNormal;
           property Index: Integer read FIndex;
           constructor Create (AIndex: Integer; ANormal: TVector3f);
    end;

    // contains a pair (position (rounded) / normal)
    // this is used for direct point lighting
    // the position is rounded in order to reduce the count of point light nodes
    TPointLightNode = class (TObject)
        private
           FNormal, FPosition: TVector3f;
           FIndex: Integer;
        public
           property Normal: TVector3f read FNormal;
           property Position: TVector3f read FPosition;
           property Index: Integer read FIndex;
           constructor Create (AIndex: Integer; ANormal, APosition: TVector3f);
    end;

    // contains the link between the node, the parallel-light-node and the point-light-node
    TFaceLightNode = class (TObject)
        private
           FNodeIndex: Integer;
           FParallelLightNodeIndex: Integer;
           FPointLightNodeIndex: Integer;
           FIndex: Integer;
        public
           property NodeIndex: Integer read FNodeIndex;
           property ParallelLightNodeIndex: Integer read FParallelLightNodeIndex;
           property PointLightNodeIndex: Integer read FPointLightNodeIndex;
           property Index: Integer read FIndex;
           constructor Create (AIndex, ANodeIndex, AParallelLightNodeIndex, APointLightNodeIndex: Integer);
    end;

    { Contains:
        - an array of ParallelLightNodes. Each normal is unique
        - an array of PointLightNodes. Each pair (position/normal) is unique
        - an array of FaceLightNodes. Each pair of indices is unique
    }
    TLightNodeList = class (TObject)
        private
           FFaceLightMap: DMap;
           FFaceLightList: TObjectList;
           FPointLightMap: DMap;
           FPointLightList: TObjectList;
           FParallelLightMap: DMap;
           FParallelLightList: TObjectList;
           function GetFaceLightNodeCount: Integer;
           function GetFaceLightNode (Index: Integer): TFaceLightNode;
           function GetPointLightNodeCount: Integer;
           function GetPointLightNode (Index: Integer): TPointLightNode;
           function GetParallelLightNodeCount: Integer;
           function GetParallelLightNode (Index: Integer): TParallelLightNode;

           function AddPointLightNode (ARoundedPosition, ANormal: TVector3f): TPointLightNode;
           function AddParallelLightNode (ANormal: TVector3f): TParallelLightNode;
        public
           property FaceLightNodeCount: Integer read GetFaceLightNodeCount;
           property FaceLightNodes[Index: Integer]: TFaceLightNode read GetFaceLightNode;
           property PointLightNodeCount: Integer read GetPointLightNodeCount;
           property PointLightNodes[Index: Integer]: TPointLightNode read GetPointLightNode;
           property ParallelLightNodeCount: Integer read GetParallelLightNodeCount;
           property ParallelLightNodes[Index: Integer]: TParallelLightNode read GetParallelLightNode;
           constructor Create;
           destructor Destroy; override;

           // DoRound: Flag if model is not tilable, if it is tileable, we may not round too close
           function AddFaceLightNode (ANodeIndex: Integer; APosition, ANormal: TVector3f; ModelIsTileable: Boolean): TFaceLightNode;
    end;

implementation

    constructor TParallelLightNode.Create (AIndex: Integer; ANormal: TVector3f);
    begin
        FIndex := AIndex;
    end;

    constructor TPointLightNode.Create (AIndex: Integer; ANormal, APosition: TVector3f);
    begin
        FIndex := AIndex;
        FNormal := ANormal;
        FPosition := APosition;
    end;

    constructor TFaceLightNode.Create (AIndex, ANodeIndex, AParallelLightNodeIndex, APointLightNodeIndex: Integer);
    begin
        FIndex := AIndex;
        FNodeIndex := ANodeIndex;
        FParallelLightNodeIndex := AParallelLightNodeIndex;
        FPointLightNodeIndex := APointLightNodeIndex;
    end;

    constructor TLightNodeList.Create;
    begin
       FFaceLightMap := DMap.Create;
       FFaceLightList := TObjectList.Create;
       FPointLightMap := DMap.Create;
       FPointLightList := TObjectList.Create;
       FParallelLightMap := DMap.Create;
       FParallelLightList := TObjectList.Create;
    end;

    destructor TLightNodeList.Destroy;
    begin
       FFaceLightMap.Free;
       FFaceLightList.Free;
       FPointLightMap.Free;
       FPointLightList.Free;
       FParallelLightMap.Free;
       FParallelLightList.Free;
    end;

    function TLightNodeList.GetFaceLightNodeCount: Integer;
    begin
        result := FFaceLightList.Count;
    end;

    function TLightNodeList.GetFaceLightNode (Index: Integer): TFaceLightNode;
    begin
        result := FFaceLightList[Index] as TFaceLightNode;
    end;

    function TLightNodeList.GetPointLightNodeCount: Integer;
    begin
        result := FPointLightList.Count;
    end;

    function TLightNodeList.GetPointLightNode (Index: Integer): TPointLightNode;
    begin
        result := FPointLightList[Index] as TPointLightNode;
    end;

    function TLightNodeList.GetParallelLightNodeCount: Integer;
    begin
        result := FParallelLightList.Count;
    end;

    function TLightNodeList.GetParallelLightNode (Index: Integer): TParallelLightNode;
    begin
        result := FParallelLightList[Index] as TParallelLightNode;
    end;

    function TLightNodeList.AddFaceLightNode (ANodeIndex: Integer; APosition, ANormal: TVector3f; ModelIsTileable: Boolean): TFaceLightNode;
    var
       Hash: String;
       Iter: DIterator;
       PointLightNode: TPointLightNode;
       ParallelLightNode: TParallelLightNode;
       Index: Integer;
       ARoundedPosition: TVector3f;
    begin
       if ModelIsTileable then begin
           For Index := 0 to 2 do // round data
               ARoundedPosition[Index] := round(APosition[Index] * 10) / 10;
       end else begin
           For Index := 0 to 2 do // round data
               ARoundedPosition[Index] := round(APosition[Index] * 2) / 2;
       end;

       PointLightNode := AddPointLightNode(ARoundedPosition, ANormal);
       ParallelLightNode := AddParallelLightNode (ANormal);

       Hash := Format ('%d %d %d', [ANodeIndex, PointLightNode.Index, ParallelLightNode.Index]);
       Iter := FFaceLightMap.locate([Hash]);
       if atEnd (Iter) then begin
          result := TFaceLightNode.Create(FFaceLightList.Count, ANodeIndex, ParallelLightNode.Index, PointLightNode.Index);
          FFaceLightMap.putPair([Hash, result]);
          FFaceLightList.Add(result);
       end else
          result := getObject (Iter) as TFaceLightNode;
    end;

    function TLightNodeList.AddPointLightNode (ARoundedPosition, ANormal: TVector3f): TPointLightNode;
    var
       Hash: String;
       Iter: DIterator;
    begin
       Hash := Format ('%.2f %.2f %.2f %.2f %.2f %.2f', [ ARoundedPosition[0], ARoundedPosition[1], ARoundedPosition[2], ANormal[0], ANormal[1], ANormal[2] ]);
       Iter := FPointLightMap.locate([Hash]);
       if atEnd (Iter) then begin
          result := TPointLightNode.Create(FPointLightList.Count, ANormal, ARoundedPosition);
          FPointLightMap.putPair([Hash, result]);
          FPointLightList.Add(result);
       end else
          result := getObject (Iter) as TPointLightNode;
    end;

    function TLightNodeList.AddParallelLightNode (ANormal: TVector3f): TParallelLightNode;
    var
       Hash: String;
       Iter: DIterator;
    begin
       Hash := Format ('%.2f %.2f %.2f', [ANormal[0], ANormal[1], ANormal[2] ]);
       Iter := FParallelLightMap.locate([Hash]);
       if atEnd (Iter) then begin
          result := TParallelLightNode.Create(FParallelLightList.Count, ANormal);
          FParallelLightMap.putPair([Hash, result]);
          FParallelLightList.Add(result);
       end else
          result := getObject (Iter) as TParallelLightNode;
    end;


end.
