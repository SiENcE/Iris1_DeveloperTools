unit Unit_LightNodes;
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
