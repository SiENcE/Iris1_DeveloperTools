unit Unit_XmlNode;
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

uses Classes, LibXmlComps, LibXmlParser, dialogs, SysUtils, Contnrs, StrUtils;

type
  TXMLNode = class (TObject)
      private
         FSection: String;
         FValue: String;
         FChildNodes: TObjectList;
         FAttributeString: String;
         FCommentString: String;
         function GetChild (Index: Integer): TXMLNode;
         function GetChildCount: Integer;
      public
        property ChildCount: Integer read GetChildCount;
        property Children [Index: Integer]: TXMLNode read GetChild; default;
        property Section: String read FSection write FSection;
        property Value: String read FValue write FValue;
        constructor Create;
        destructor Destroy; override;
        function FindChild (Section: String; FailIfNotExisting: Boolean = False): TXMLNode;
        function GetChildValue (Section: String): String;
        function AddChild (Section: String): TXMLNode;
        procedure SetChildValue (Section: String; Value: String);
        procedure Save (Strings: TStrings; Level: Integer);
  end;

  TXMLFile = class (TObject)
      private
        FRootNode: TXMLNode;
        FCurrentNode: TXMLNode;
        FNodeStack: TStack;
        procedure OnStartTag (Sender : TObject; TagName : STRING; Attributes : TAttrList);
        procedure OnEndTag (Sender : TObject; TagName : STRING);
        procedure OnContent (Sender : TObject; Content : STRING);
        procedure OnComment (Sender : TObject; Comment : STRING);
      public
        property RootNode: TXMLNode read FRootNode;
        constructor Create (AStream: TStream; ACount: Integer);
        destructor Destroy; override;
  end;

implementation

   constructor TXMLNode.Create;
   begin
      FChildNodes := TObjectList.Create;
   end;

   destructor TXMLNode.Destroy;
   begin
      FChildNodes.Free;
   end;

   function TXMLNode.GetChild (Index: Integer): TXMLNode;
   begin
      result := FChildNodes[Index] as TXMLNode;
   end;

   function TXMLNode.GetChildCount: Integer;
   begin
      result := FChildNodes.Count;
   end;

   function TXMLNode.FindChild (Section: String; FailIfNotExisting: Boolean): TXMLNode;
   var
     Index: Integer;
   begin
      result := nil;

      Section := Section;
      For Index := 0 to ChildCount - 1 do
          if Children[Index].Section = Section then
              result := Children[Index];

      if not Assigned (result) and FailIfNotExisting then
          raise Exception.Create ('Value ' + Section + ' not found');
   end;

   function TXMLNode.GetChildValue (Section: String): String;
   var
     ANode: TXMLNode;
   begin
      ANode := FindChild (Section, false);
      if Assigned (ANode) then
          result := ANode.Value
      else
          result := '';
   end;

   function TXMLNode.AddChild (Section: String): TXMLNode;
   begin
      result := FindChild (Section);
      if not Assigned (result) then begin
          result := TXMLNode.Create;
          result.FSection := Section;
          FChildNodes.Add(result);
      end;
   end;

   procedure TXMLNode.SetChildValue (Section: String; Value: String);
   var
     ANode: TXMLNode;
   begin
      ANode := AddChild (Section);
      ANode.FValue := Value;
   end;

   procedure TXMLNode.Save (Strings: TStrings; Level: Integer);
   var
     Prefix: String;
     Index: Integer;
   begin
      Assert (Assigned (Strings));
      Prefix := DupeString (#9, Level);
      if ChildCount = 0 then begin
        Strings.Add (Prefix + '<' + Section + FAttributeString + '>' + Value + '</' + Section + '>');
        if FCommentString <> '' then begin
            Strings.Add ('');
            Strings.Add (Prefix + FCommentString);
        end;
      end else begin
        Strings.Add (Prefix + '<' + Section + FAttributeString + '>');
        if FCommentString <> '' then
            Strings.Add (Prefix + #9 + FCommentString);
        For Index := 0 to ChildCount - 1 do
            Children[Index].Save(Strings, Level + 1);
        Strings.Add (Prefix + '</' + Section + '>');
        Strings.Add ('');
      end;


   end;

   constructor TXMLFile.Create (AStream: TStream; ACount: Integer);
   var
      XMLScanner: TEasyXmlScanner;
      DummyBuffer: PChar;
   begin
       XMLScanner := TEasyXmlScanner.Create (nil);
       try
          GetMem (DummyBuffer, ACount + 1);
          try
              FillChar (DummyBuffer^, ACount + 1, 0);
              AStream.Read(DummyBuffer^, ACount);
              XMLScanner.LoadFromBuffer (DummyBuffer);
          finally
              FreeMem (DummyBuffer);
          end;

          FNodeStack := TStack.Create;
          try
              FCurrentNode := nil;
              XMLScanner.OnStartTag := OnStartTag;
              XMLScanner.OnEndTag := OnEndTag;
              XMLScanner.OnComment := OnComment;
              XMLScanner.OnContent := OnContent;
              XMLScanner.Execute;
          finally
              FNodeStack.Free;
              FNodeStack := nil;
          end;

          Assert (not Assigned (FCurrentNode));

       finally
          XMLScanner.Free;
       end;

       if not Assigned (FRootNode) then begin
          FRootNode := TXMLNode.Create;
          FRootNode.FSection := 'root';
       end;
    end;

    destructor TXMLFile.Destroy;
    begin
       FRootNode.Free;
    end;

    procedure TXMLFile.OnStartTag (Sender : TObject; TagName : STRING; Attributes : TAttrList);
    var
      Node: TXMLNode;
      Index: Integer;
    begin
        Assert (Assigned (FNodeStack));
        Node := TXMLNode.Create;
        Node.FSection := TagName;

        For Index := 0 to Attributes.Count - 1 do
             Node.FAttributeString := Node.FAttributeString + ' ' + Attributes.Name(Index) + '="' + Attributes.Value(Index) + '"';

        if Assigned (FCurrentNode) then begin
            FNodeStack.Push(FCurrentNode);
            FCurrentNode.FChildNodes.Add(Node);
        end else begin
            FRootNode := Node;
        end;
        FCurrentNode := Node;
    end;

    procedure TXMLFile.OnEndTag (Sender : TObject; TagName : STRING);
    begin
        Assert (Assigned (FNodeStack));
        Assert (Assigned (FCurrentNode));
        Assert (FCurrentNode.FSection = TagName);
        if FNodeStack.Count > 0 then
            FCurrentNode := FNodeStack.Pop
        else
            FCurrentNode := nil;
    end;

    procedure TXMLFile.OnContent (Sender : TObject; Content : STRING);
    begin
        Assert (Assigned (FCurrentNode));
        FCurrentNode.FValue := FCurrentNode.FValue + Content;
    end;

    procedure TXMLFile.OnComment (Sender : TObject; Comment : STRING);
    begin
        if (Assigned (FCurrentNode)) then begin
            if FCurrentNode.ChildCount > 0 then
                FCurrentNode.Children[FCurrentNode.ChildCount - 1].FCommentString :=
                    FCurrentNode.Children[FCurrentNode.ChildCount - 1].FCommentString + Comment
            else
                FCurrentNode.FCommentString := FCurrentNode.FCommentString + Comment;
        end;
    end;

end.
