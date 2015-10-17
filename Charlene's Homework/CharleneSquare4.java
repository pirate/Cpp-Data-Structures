public class Square4 {
  public static void main(String[] args) {
    int length = Integer.parseInt(args[0]) - 1;
    int x = Integer.parseInt(args[1]);
    int y = Integer.parseInt(args[2]);
   
    boolean inyaxis = true;

    // Stage 0. make sure the numbers are valid

      if (x < 0 || y < 0) {
        System.out.println("Numbers are invalid, you idiot! Only give starting co-ordinates > 0.");
        return;
      }

    // Stage 1. decide how big to make the grid to fit our square

      int gridSizeX = 15;
      int gridSizeY = 15;

      if (x+length > gridSizeX) {
        gridSizeX = x + length;
      }

      if (y+length > gridSizeY) {
        gridSizeY = y + length;
      }

    // Stage 3. Draw the top line of the grid (which is just a little arrow)
      System.out.println("^");


    // Stage 4. Draw the body of the grid, from top down, left to right

      // start drawing top-down
      for (int currentY = gridSizeY; currentY >= 0; currentY--) {
        System.out.print("|");
        for (int currentX = 0; currentX <= gridSizeX; currentX++) {
          // As we're moving our cursor from top-down left to right
          // we decide when to start drawing #'s and when to draw blank spaces

          // if we're lower than the top of the square, and higher than the bottom and
          // if we're to the right of the leftmost edge, and to the left of the rightmost edge
          if ((currentY <= y+length && currentY >= y) && (currentX >= x && currentX <= x+length)) {
            System.out.print("#"); // draw #s
          }
          else {
            System.out.print(" "); // otherwise, we're not inside the bounds of the square, so we just draw empty space as a placeholder
          }
        }
        System.out.println(""); // end the row with a new line
      }

    // Stage 5. Draw the bottom x axis, from left to right
      System.out.print("+");
      for (int currentX = 0; currentX <= gridSizeX; currentX++) {
        System.out.print("-");
      }
      System.out.println(">");

    return;
  };
};
