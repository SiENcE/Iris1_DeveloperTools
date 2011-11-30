object ProgressForm: TProgressForm
  Left = 245
  Top = 148
  BorderStyle = bsNone
  Caption = 'ProgressForm'
  ClientHeight = 49
  ClientWidth = 517
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 14
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 517
    Height = 49
    Align = alClient
    BorderStyle = bsSingle
    TabOrder = 0
    object InfoText: TLabel
      Left = 8
      Top = 4
      Width = 497
      Height = 14
      Alignment = taCenter
      AutoSize = False
      BiDiMode = bdLeftToRight
      Caption = 'InfoText'
      ParentBiDiMode = False
    end
    object ProgressBar1: TProgressBar
      Left = 8
      Top = 22
      Width = 497
      Height = 17
      TabOrder = 0
    end
  end
end
