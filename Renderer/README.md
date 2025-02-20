# Renderer

    Imamo problem sa GUI uzima 40% cpu;
    Nedostaje mi 

### Renderer radi tako sto pri pozivu konstruktora generise novi ID(VAO)

    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);

### Definisemo vertex buffer konstruktorom gde prosledjujemo 2 parametra: 1. polje u koje smo definisali tacke(vertexe) i 2. velicinu koju zahvatamo u tom nizu u bajtovima.

    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

### Definisemo VertexBufferLayout koji nam sluzi da povezemo tacke onog cega zelimo da nacrtamo i klase koja po zadatim parametrima iz VertexBuffera racuna razmak svakog podatka i definise VAO ili VertexArray.

    m_Elements.push_back({type, count, GL_FALSE});
    this->m_Stride += count * VertexBufferElement::getSizeOfType(type);
    Bind();
    vb.Bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), 0);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }

### Povezujemo sad VAO sa IndexBufferom tako sto napravimo objekat po konstruktoru i prosledjujemo 1. polje u kojem smo definisali redosled i broj tacaka koje crtamo.

    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

# END RENDERER