import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class dimtrans {
    /**
     * @param args
     */
    public static void main(String[] args) {
        if (args == null || args.length < 1) {
            // 1.875 1024x600
            // 2.4 800x450
            System.out.println("Usage: java dimtrans scale xxx/dimen.xml");
            return;
        }

        final float scale = parseScale(args[0]);
        if (Float.compare(scale, 0.0f) <= 0) {
            System.err.println("Invalid scale - scale must be > 0");
            return;
        }

        try {
            final File file = new File(args[1]);
            final DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            final Document src = builder.parse(file);
            final Document dst = builder.newDocument();
            final Element root = createDocumentElement(dst);

            final NodeList nodes = src.getDocumentElement().getChildNodes();
            for (int i = 0, length = nodes.getLength(); i < length; ++i) {
                final Node newNode, node = nodes.item(i);
                switch (node.getNodeType()) {
                case Node.ELEMENT_NODE:
                    newNode = createElement(dst, (Element)node, scale);
                    break;

                case Node.TEXT_NODE:
                    newNode = dst.createTextNode(node.getTextContent());
                    break;

                case Node.COMMENT_NODE:
                    newNode = dst.createComment(node.getTextContent());
                    break;

                default:
                    throw new IllegalStateException("Unknown node type - " + node.getNodeType());
                }

                root.appendChild(newNode);
            }

            final File outFile = new File(file.getParent(), "trans-" + file.getName());
            saveDocument(dst, outFile);
            System.out.println("Transform OK - scale = " + scale + ", outputFile = " + outFile.getAbsolutePath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static float parseScale(String param) {
        try {
            return Float.parseFloat(param);
        } catch (Exception e) {
            return 0;
        }
    }

    private static String transDimension(String value, float scale) {
        if (value == null) {
            return value;
        }

        final int length = value.length();
        if (length == 0 || value.charAt(0) == '-') {
            return value;
        }

        int end = length - 1;
        for (; end >= 0; --end) {
            if (Character.isDigit(value.charAt(end))) {
                break;
            }
        }

        int number = (int)(Float.parseFloat(value.substring(0, end + 1)) / scale);
        if (number <= 0) {
            number = 1;
        }

        return (number + "px");
    }

    private static Element createDocumentElement(Document document) {
        final Element root = document.createElement("resources");
        document.appendChild(root);
        document.setXmlStandalone(true);
        return root;
    }

    private static Element createElement(Document document, Element src, float scale) {
        final Element element = document.createElement("dimen");
        element.setAttribute("name", src.getAttribute("name"));
        element.setTextContent(transDimension(src.getTextContent(), scale));
        return element;
    }

    private static void saveDocument(Document document, File outFile) {
        try {
            final Transformer transformer = TransformerFactory.newInstance().newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty(OutputKeys.VERSION, "1.0");
            transformer.setOutputProperty(OutputKeys.ENCODING, "utf-8");
            transformer.transform(new DOMSource(document), new StreamResult(outFile));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
