import asyncio
import requests
import chromadb
from mcp.server import Server
from mcp.server.stdio import stdio_server
from mcp import types

# --- Connect to your ChromaDB ---
chroma_client = chromadb.PersistentClient(path=r"C:\Users\uma_r\cpp-assistant\chroma_db")
collection = chroma_client.get_collection(name="llama_index")

# --- Embed query using nomic-embed-text via Ollama ---
def embed_query(text: str) -> list[float]:
    response = requests.post(
        "http://localhost:11434/api/embeddings",
        json={"model": "nomic-embed-text", "prompt": text}
    )
    response.raise_for_status()
    return response.json()["embedding"]

# --- Search ChromaDB ---
def search_chromadb(query: str, n_results: int = 5) -> str:
    query_embedding = embed_query(query)
    results = collection.query(
        query_embeddings=[query_embedding],
        n_results=n_results,
        include=["documents", "metadatas"]
    )

    documents = results.get("documents", [[]])[0]
    metadatas = results.get("metadatas", [[]])[0]

    if not documents:
        return "No relevant context found in the database."

    output = []
    for i, (doc, meta) in enumerate(zip(documents, metadatas)):
        source = meta.get("file_name") or meta.get("source") or "Unknown source"
        output.append(f"[{i+1}] Source: {source}\n{doc}")

    return "\n\n---\n\n".join(output)

# --- MCP Server setup ---
server = Server("cpp-assistant-rag")

@server.list_tools()
async def list_tools() -> list[types.Tool]:
    return [
        types.Tool(
            name="search_docs",
            description=(
                "Search the C++ assistant knowledge base for relevant documentation, "
                "code snippets, or explanations. Use this for any question about the application."
            ),
            inputSchema={
                "type": "object",
                "properties": {
                    "query": {
                        "type": "string",
                        "description": "The search query to look up in the knowledge base"
                    },
                    "n_results": {
                        "type": "integer",
                        "description": "Number of results to return (default: 5)",
                        "default": 5
                    }
                },
                "required": ["query"]
            }
        )
    ]

@server.call_tool()
async def call_tool(name: str, arguments: dict) -> list[types.TextContent]:
    if name == "search_docs":
        query = arguments.get("query", "")
        n_results = arguments.get("n_results", 5)
        result = search_chromadb(query, n_results)
        return [types.TextContent(type="text", text=result)]
    else:
        return [types.TextContent(type="text", text=f"Unknown tool: {name}")]

# --- Run the server ---
async def main():
    async with stdio_server() as (read_stream, write_stream):
        await server.run(read_stream, write_stream, server.create_initialization_options())

if __name__ == "__main__":
    asyncio.run(main())
