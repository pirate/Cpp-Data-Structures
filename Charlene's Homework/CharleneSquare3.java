public class Square3 {
 public static void main(String[] args) {
  int length = Integer.parseInt(args[0]);
  int x = Integer.parseInt(args[1]);
  int y = Integer.parseInt(args[2]);
 
  boolean inyaxis = true;
 
  while(inyaxis) {
    for (int yaxis = 15; yaxis>=0; yaxis--) {
      if (yaxis==15) {
        System.out.println("^");
      }
      else if(yaxis==length-1){
        for(int c=1; c<=length; c++){
        System.out.print("|");
          for(int d=0; d<x; d++){
            System.out.print(" ");
           }
          for(int b=1; b<=length;b++){
            if(b==length){
              System.out.println("#");
            }
            else{
            System.out.print("#");
            }
          }
        }
      }
      else if(yaxis<length-1){
        for(int h=1; h<=y; h++){
            System.out.println("|");
            if(h==y){
              inyaxis=false;
            }
         }
        }
      else if(yaxis>length-1){
        for(int j=1; j<=15-length-y; j++){
          System.out.println("|");
        }
        yaxis=length;
      }
      }
    }
    //x-axis
    for(int xaxis = 0; xaxis<=17; xaxis++){
      if(xaxis==0){
        System.out.print("+");
      }
      else if(xaxis<16){
        System.out.print("-");
      }
      else if(xaxis==16 && x+length<=15){
        System.out.println(">");
        break;
      }
      else if(xaxis==16 && x+length>15){
        for(int h=1; h<=length; h++){
            System.out.print("-");
            if(h==length){
              System.out.println(">");
            }
         }
        break;
      }
       }
      }
   }
