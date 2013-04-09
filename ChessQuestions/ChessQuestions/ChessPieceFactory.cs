//////////////////////////////////////////////////////////////////////////////
//
//  ChessPieceFactory.cs
//
// 
// © 2008 Microsoft Corporation. All Rights Reserved.
//
// This file is licensed as part of the Silverlight 2 SDK, for details look here: http://go.microsoft.com/fwlink/?LinkID=111970&clcid=0x409
//
///////////////////////////////////////////////////////////////////////////////
namespace wp7chess
{
	using System;
	using System.IO;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Markup;
    using System.Reflection;
    using System.Windows.Resources;

	public class Piece : UserControl
	{
		private Canvas graphic;

		public Piece(string resourceName)
		{
            StreamResourceInfo res = Application.GetResourceStream(new Uri("wp7chess;component/Pieces/" + resourceName, UriKind.Relative));
            StreamReader reader = new StreamReader(res.Stream);

			//Stream stream = this.GetType().Assembly.GetManifestResourceStream(resourceName);
			//StreamReader reader = new StreamReader(stream);
			this.graphic = (Canvas)XamlReader.Load(reader.ReadToEnd());

			this.Content = this.graphic;
			this.IsHitTestVisible = false;
		}
	}

	public class ChessPieceFactory
	{
		private static readonly string[] pieceResourceName = new string[] { null, "BlackPawn.xaml", "BlackKnight.xaml", "BlackBishop.xaml", "BlackRook.xaml", "BlackQueen.xaml", "BlackKing.xaml", null,
                       null, "WhitePawn.xaml", "WhiteKnight.xaml", "WhiteBishop.xaml", "WhiteRook.xaml", "WhiteQueen.xaml", "WhiteKing.xaml", null };

		public Piece CreatePiece(int pieceType)
		{
			string pieceName = ChessPieceFactory.pieceResourceName[pieceType & 0xF];
			if (pieceName != null)
			{
				return new Piece(pieceName);
			}
			return null;
		}
	}
}