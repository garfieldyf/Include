    private static final class Node {
        int value;
        Node next;

        public Node(int value) {
            this.value = value;
        }
    }

    private static Node createNode(int length) {
//        Node head;
//        for (int i = 1; i <= length; ++i) {
//            Node n = new Node(i);
//            n.next = head;
//            head = n;
//        }

        Node last = new Node(1);
        Node head = last;
        for (int i = 2; i <= length; ++i) {
            Node n = new Node(i);
            n.next = head;
            head = n;
        }

        last.next = head;
        return head;
    }

    private static void printNode(Node head, int length) {
        StringBuilder s = new StringBuilder();
//        Node p = head;
//        do {
//            s.append(p.value).append("->");
//            p = p.next;
//        } while (p != head);

        for (int i = 0; i < 7; ++i) {
            s.append(head.value).append("->");
            head = head.next;
        }

        Log.i("aaaa", s.toString());
    }

    private static void reverseNode() {
        Node head = createNode(4);
        printNode(head, 5);

        Node prev = null, next;
        Node temp = head;
        do {
            next = temp.next;
            temp.next = prev;
            prev = temp;
            temp = next;
        } while (temp != head);

//        while (head != null) {
//            next = head.next;
//            head.next = prev;
//            prev = head;
//            head = next;
//        }

        head.next = prev;
        head = prev;
        printNode(head, 5);
    }
