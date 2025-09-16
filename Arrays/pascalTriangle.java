package Arrays;
import java.util.*;

class Solution {
    private List<Integer> generateRows(int row) {
        List<Integer> temp = new ArrayList<>();
        temp.add(1);
        int val = 1;

        for (int i = 1; i < row; i++) {
            val = val * (row - i);
            val = val / i;
            temp.add(val);
        }

        return temp;
    }

    public List<List<Integer>> generate(int numRows) {
        List<List<Integer>> ans = new ArrayList<>();

        for (int i = 1; i <= numRows; i++) {
            ans.add(generateRows(i));
        }

        return ans;
    }
}
