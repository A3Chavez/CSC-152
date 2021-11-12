public class Le {

    // Read the four bytes begining at p and assemble them little-endian
    public static int load_int_little(byte[] p) {   
        int i = ((0xFF & p[3]) << 24) | ((0xFF & p[2]) << 16) |
            ((0xFF & p[1]) << 8) | (0xFF & p[0]);
            
        //int i = ((0xFF & p1) << 24) | ((0xFF & p2) << 16) |
          //  ((0xFF & p3) << 8) | (0xFF & p4);
            
        /*
        int leftmost_byte = ((0x00000000 | p[0]) & 0x000000FF) >> 0;
        int left_mid_byte = ((0x00000000 | p[1]) & 0x0000FF00) >> 8;
        int right_mid_byte = (p[2] & 0x00FF0000) >> 16;
        int rightmost_byte = (p[3] & 0xFF000000) >> 24;
        
        leftmost_byte <<= 24;
        left_mid_byte <<= 16;
        right_mid_byte <<= 8;
        rightmost_byte <<= 0;
 */
        //int result = (leftmost_byte | left_mid_byte | right_mid_byte | rightmost_byte);
        
          System.out.println("int in hex: "+Integer.toHexString(i)); // prints "fffefdfc"
          return i;
    }

    public static void main(String args[]) {
        byte[] p = {0x04,0x03,0x02,0x01};
        int x = load_int_little(p);
        System.out.println("INT is "+x);
    }

}