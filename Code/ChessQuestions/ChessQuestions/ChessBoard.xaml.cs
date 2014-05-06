//////////////////////////////////////////////////////////////////////////////
//
//  ChessBoard.xaml.cs
//
// 
// © 2008 Microsoft Corporation. All Rights Reserved.
//
// This file is licensed as part of the Silverlight 2 SDK, for details look here: http://go.microsoft.com/fwlink/?LinkID=111970&clcid=0x409
//
///////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Collections.Generic;

namespace ChessQuestions
{
	public class Square
	{
		private ChessBoard parent;
		private Rectangle squareRect;
		private int row, column;
		private bool highlight;

		public Square(ChessBoard parent, int row, int column)
		{
			this.squareRect = new Rectangle();
			this.squareRect.Fill = ((row + column) % 2) == 0 ? new SolidColorBrush(Color.FromArgb(255, 193, 124, 66)) : new SolidColorBrush(Color.FromArgb(255, 231, 208, 167));
			this.UpdateHighlight();
			Grid.SetRow(this.squareRect, row);
			Grid.SetColumn(this.squareRect, column);

			this.squareRect.MouseLeftButtonDown += this.OnMouseLeftButtonDown;

			this.parent = parent;
			this.parent.LayoutRoot.Children.Add(this.squareRect);

			this.row = row;
			this.column = column;
		}

		public bool Highlight
		{
			get { return this.highlight; }
			set
			{
				this.highlight = value;
				this.UpdateHighlight();
			}
		}

		private void UpdateHighlight()
		{
			if (this.highlight)
			{
				this.squareRect.StrokeThickness = 10;
				this.squareRect.Stroke = new SolidColorBrush(Colors.Black);
			}
			else
			{
				this.squareRect.StrokeThickness = 0.5;
				this.squareRect.Stroke = new SolidColorBrush(Colors.Black);
			}
		}

		private void OnMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
		{
			this.parent.OnSquareClicked(this.row, this.column);
		}
	}

	public partial class ChessBoard : UserControl
	{
		private ChessPieceFactory chessPieceFactory = new ChessPieceFactory();
		private Square[][] squares = new Square[8][];
		private List<Piece> pieces = new List<Piece>();
		private List<Piece> oldPieces;
		private Position position;
		private int previousSquareSelected = -1;

		public delegate void UserMoveHandler(ushort move);
		public event UserMoveHandler UserMove;

		public delegate void AnimationCompletedHandler();
		public event AnimationCompletedHandler AnimationCompleted;

		public ChessBoard()
		{
			// Required to initialize variables
			this.InitializeComponent();

			// Set up the board squares
			for (int row = 0; row < 8; row++)
			{
				RowDefinition rowDefinition = new RowDefinition();
				rowDefinition.Height = new GridLength(1.0 / 8.0, GridUnitType.Star);
				this.LayoutRoot.RowDefinitions.Add(rowDefinition);

				ColumnDefinition columnDefinition = new ColumnDefinition();
				columnDefinition.Width = new GridLength(1.0 / 8.0, GridUnitType.Star);
				this.LayoutRoot.ColumnDefinitions.Add(columnDefinition);

				this.squares[row] = new Square[8];
				for (int col = 0; col < 8; col++)
				{
					this.squares[row][col] = new Square(this, row, col);
				}
			}

			for (int i = 0; i < 256; i++)
			{
				Piece piece = this.chessPieceFactory.CreatePiece(i);
				if (piece != null)
				{
					piece.Opacity = 0;
					this.LayoutRoot.Children.Add(piece);
				}
				this.pieces.Add(piece);
			}

			this.LayoutUpdated += this.OnChessBoardLayoutUpdated;
		}

		public Position Position
		{
			get { return this.position; }
			set
			{
				this.position = value;
				this.UpdateForNewPosition();
			}
		}

		public void OnSquareClicked(int row, int column)
		{
			int square = SquareHelper.MakeSquare(row, column);

			List<ushort> validMoves = this.position.GenerateValidMoves();
			if (validMoves.Count == 0)
			{
				return;
			}

			if (this.previousSquareSelected == -1)
			{
				// Select a square
				if (this.position.Board[square] != Pieces.Empty)
				{
					if ((this.position.Board[square] & 0x8) == position.ToMove)
					{
						this.squares[row][column].Highlight = true;
						this.previousSquareSelected = square;
//						this.StatusText = "Select square to move to";
					}
					else
					{
//						this.StatusText = String.Format("You must select a {0} piece", position.ToMove == Pieces.White ? "White" : "Red");
					}
				}
			}
			else if (this.previousSquareSelected == square)
			{
				// Deselect the square
				this.squares[row][column].Highlight = false;
				this.previousSquareSelected = -1;
//				this.StatusText = "Select piece to move";
			}
			else
			{
				// Find a move
				int from = this.previousSquareSelected;
				int to = square;

				ushort validMove = 0;
				foreach (ushort move in validMoves)
				{
					if (MoveHelper.From(move) == from &&
						MoveHelper.To(move) == to)
					{
						validMove = move;
					}
				}

				if (validMove == 0)
				{
//					this.StatusText = "Invalid move!";
					return;
				}

				this.squares[SquareHelper.GetRow(from)][SquareHelper.GetColumn(from)].Highlight = false;
				this.previousSquareSelected = -1;

				if (this.UserMove != null)
				{
					this.UserMove(validMove);
				}
			}
		}

		protected override Size ArrangeOverride(Size finalSize)
		{
			double size = Math.Min(finalSize.Width, finalSize.Height);
			return base.ArrangeOverride(new Size(size, size));
		}

		private List<byte> oldPieceList;
		private List<byte> oldBoard;
		private int currentAnimation = 0;

		public void UpdateForNewPosition()
		{
			const int duration = 400;

			this.UpdatePieceSizes();

			Storyboard animation = new Storyboard();
			animation.Duration = TimeSpan.FromMilliseconds(duration + 25);
			this.Resources.Add("tmpTimline" + this.currentAnimation++, animation);

			for (int i = 0; i < this.position.PieceList.Length; i++)
			{
				byte oldPieceLocation = 0xFF;
				if (this.oldPieceList != null)
				{
					oldPieceLocation = this.oldPieceList[i];
				}

				byte newPieceLocation = this.position.PieceList[i];

				Piece oldPiece = oldPieceLocation != 0xFF ? this.pieces[this.oldBoard[oldPieceLocation]] : null;
				Piece newPiece = newPieceLocation != 0xFF ? this.pieces[this.position.Board[newPieceLocation]] : null;

				if (oldPieceLocation != newPieceLocation)
				{
					if (newPieceLocation == 0xFF)
					{
						// Piece was captured
						DoubleAnimation opacityAnimation = new DoubleAnimation();
						Storyboard.SetTarget(opacityAnimation, oldPiece);
						Storyboard.SetTargetProperty(opacityAnimation, new PropertyPath("Opacity"));
						opacityAnimation.Duration = TimeSpan.FromMilliseconds(duration);
						opacityAnimation.To = 0;

						animation.Children.Add(opacityAnimation);
					}
					else if (oldPieceLocation == 0xFF)
					{
						// Piece was created (game reset, undo)

						DoubleAnimation opacityAnimation = new DoubleAnimation();
						Storyboard.SetTarget(opacityAnimation, newPiece);
                        Storyboard.SetTargetProperty(opacityAnimation, new PropertyPath("Opacity"));
						opacityAnimation.Duration = TimeSpan.FromMilliseconds(duration);
						opacityAnimation.To = 1;

						Grid.SetRow(newPiece, SquareHelper.GetRow(newPieceLocation));
						Grid.SetColumn(newPiece, SquareHelper.GetColumn(newPieceLocation));

						animation.Children.Add(opacityAnimation);
					}
					else
					{
						if (oldPiece == newPiece)
						{
							// Piece just moved
							Grid.SetRow(newPiece, SquareHelper.GetRow(newPieceLocation));
							Grid.SetColumn(newPiece, SquareHelper.GetColumn(newPieceLocation));

							DoubleAnimation yAnimation = new DoubleAnimation();
							Storyboard.SetTarget(yAnimation, ((TransformGroup)newPiece.RenderTransform).Children[0]);
							Storyboard.SetTargetProperty(yAnimation, new PropertyPath("Y"));

							yAnimation.Duration = TimeSpan.FromMilliseconds(duration);
							yAnimation.From = (SquareHelper.GetRow(oldPieceLocation) - SquareHelper.GetRow(newPieceLocation)) * 100;
							yAnimation.To = 0;

							animation.Children.Add(yAnimation);

							DoubleAnimation xAnimation = new DoubleAnimation();
							Storyboard.SetTarget(xAnimation, ((TransformGroup)newPiece.RenderTransform).Children[0]);
							Storyboard.SetTargetProperty(xAnimation,new PropertyPath("X"));

							xAnimation.Duration = TimeSpan.FromMilliseconds(duration);
							xAnimation.From = (SquareHelper.GetColumn(oldPieceLocation) - SquareHelper.GetColumn(newPieceLocation)) * 100;
							xAnimation.To = 0;

							animation.Children.Add(xAnimation);
						}
						else
						{
							// Promotion
							DoubleAnimation opacityAnimation = new DoubleAnimation();
							Storyboard.SetTarget(opacityAnimation, newPiece);
							Storyboard.SetTargetProperty(opacityAnimation,new PropertyPath ("Opacity"));
							opacityAnimation.Duration = TimeSpan.FromMilliseconds(duration);
							opacityAnimation.To = 1;

							Grid.SetRow(newPiece, SquareHelper.GetRow(newPieceLocation));
							Grid.SetColumn(newPiece, SquareHelper.GetColumn(newPieceLocation));

							animation.Children.Add(opacityAnimation);

							opacityAnimation = new DoubleAnimation();
							Storyboard.SetTarget(opacityAnimation, oldPiece);
							Storyboard.SetTargetProperty(opacityAnimation,new PropertyPath ("Opacity"));
							opacityAnimation.Duration = TimeSpan.FromMilliseconds(duration);
							opacityAnimation.To = 0;

							animation.Children.Add(opacityAnimation);
						}
					}
				}
				else
				{
					// TODO: what should be done here?
				}
			}

			// Workaround layout bug.
			this.LayoutRoot.Width--;
			this.LayoutRoot.Height++;

			animation.Completed += this.OnAnimationCompleted;
			animation.Begin();

			this.oldBoard = new List<byte>(this.position.Board);
			this.oldPieceList = new List<byte>(this.position.PieceList);
			this.oldPieces = new List<Piece>(this.pieces);
		}

		private void OnAnimationCompleted(object sender, EventArgs e)
		{
			if (this.AnimationCompleted != null)
			{
				this.AnimationCompleted();
			}
		}

		private void OnChessBoardLayoutUpdated(object sender, EventArgs e)
		{
			// Re-scale all our children pieces
			this.UpdatePieceSizes();
		}

		private double lastSize = -1;
		private void UpdatePieceSizes()
		{
			double size = (this.ActualWidth / 8) / 100.0;
			if (size == this.lastSize) return;
			this.lastSize = size;

			for (int i = 0; i < this.pieces.Count; i++)
			{
				if (this.pieces[i] != null)
				{
					TransformGroup group = new TransformGroup();
					TranslateTransform translate = new TranslateTransform();
					translate.X = 0;
					translate.Y = 0;
					group.Children.Add(translate);
					ScaleTransform scale = new ScaleTransform();
					scale.ScaleX = size;
					scale.ScaleY = size;
					group.Children.Add(scale);
					this.pieces[i].RenderTransform = group;
				}
			}
		}
	}
}