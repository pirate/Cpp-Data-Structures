import java.util.HashMap;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.*;

public class LanguageFamilyNode {
    private String name;
    private ArrayList<LanguageFamilyNode> children;

    // name getter
    public String getName() {
        return name;
    }

    // children setter
    public void addChild(LanguageFamilyNode child) {
        children.add(child);
    }

    public void printAllDescendants() {
        // for each child in the list of children
        for (LanguageFamilyNode child : children) {
            // print the child's name, then print all of its chilren, indented slightly further
            System.out.println(child.getName());
            child.printAllDescendants("  ");
        }
    }

    // overloaded to accept a prefix
    public void printAllDescendants(String prefix) {
        for (LanguageFamilyNode child : children) {
            System.out.println(prefix + child.getName());
            // same as above but append the passed-in prefix to the usual two space4s
            child.printAllDescendants("  " + prefix);
        }
    }

    // constuctor
    public LanguageFamilyNode(String nodeName) {
        name = nodeName;
        // initialize children to an empty list
        children = new ArrayList<LanguageFamilyNode>();
    }

    public static void main(String[] args) {
        // generate a few nodes, set up their higherarchy, then tell root to print its children
        System.out.println("Generating LanguageFamilyNode nodes...");
        LanguageFamilyNode root = new LanguageFamilyNode("Germanic languages");
        LanguageFamilyNode l1child1 = new LanguageFamilyNode("Germanic languages1");
        LanguageFamilyNode l1child2 = new LanguageFamilyNode("Germanic languages2");
        LanguageFamilyNode l1child3 = new LanguageFamilyNode("Germanic languages3");
        LanguageFamilyNode l2child1 = new LanguageFamilyNode("English Family");
        LanguageFamilyNode l3child1 = new LanguageFamilyNode("English");
        
        System.out.println("Arranging higherarchy...");
        l2child1.addChild(l3child1);
        l1child1.addChild(l2child1);
        root.addChild(l1child1);
        root.addChild(l1child2);
        root.addChild(l1child3);

        System.out.println("Finished tree:");
        System.out.println();
        root.printAllDescendants("  ");
    }
}
