# Advanced Architecture Patterns — Reference

Deep-dive implementation examples for DDD bounded contexts, Onion Architecture, Anti-Corruption Layers, and full project structures. Referenced from SKILL.md.

---

## Onion Architecture vs. Clean Architecture

Both enforce inward-pointing dependencies. The difference is terminology and layering granularity:

| Concern | Clean Architecture | Onion Architecture |
|---|---|---|
| Innermost ring | Entities | Domain Model |
| Second ring | Use Cases | Domain Services |
| Third ring | Interface Adapters | Application Services |
| Outermost ring | Frameworks & Drivers | Infrastructure / UI / Tests |
| Key insight | Controller is an adapter | Application Services = Use Cases |

Onion Architecture makes the Domain Services layer explicit — it hosts pure domain logic that spans multiple entities but has no I/O. A domain service such as `PricingService` applies bulk discount rules without touching ports or adapters.

---

## Anti-Corruption Layer (ACL)

When the `Ordering` context must fetch product data from the `Catalog` context, it should never use `Catalog`'s domain model directly. An ACL translates between the two models: `Ordering` defines its own `CatalogClientPort` interface and a local `ProductSnapshot` value object; an adapter implements the port by calling Catalog's API and mapping the response into `ProductSnapshot`. In tests, a stub implementation of the port replaces the adapter — no HTTP required.

---

## Context Map — Relationships Between Bounded Contexts

```
┌─────────────────────────────────────────────────────────────────┐
│                        E-Commerce System                         │
│                                                                  │
│   ┌─────────────┐   Open Host   ┌─────────────────────────┐    │
│   │  Identity   │──────────────▶│        Ordering          │    │
│   │  Context    │               │  (uses CustomerId VO,    │    │
│   │             │               │   not User entity)       │    │
│   └─────────────┘               └─────────────────────────┘    │
│                                          │ ACL                   │
│                                          ▼                       │
│                                 ┌─────────────────┐             │
│   ┌─────────────┐  Shared       │    Catalog      │             │
│   │  Payments   │  Kernel       │    Context      │             │
│   │  Context    │◀─────────────▶│                 │             │
│   │             │  (Money VO)   └─────────────────┘             │
│   └─────────────┘                                               │
└─────────────────────────────────────────────────────────────────┘

Relationship types:
  Open Host Service  — upstream provides a stable API for many downstream contexts
  ACL (Anti-Corruption Layer) — downstream translates upstream model to its own
  Shared Kernel     — two contexts share a small, explicitly governed sub-model
  Conformist        — downstream adopts upstream model as-is (last resort)
```

---

## Dependency Injection Wiring — Infrastructure Layer

All the abstract interfaces are wired to concrete implementations in the infrastructure layer (or a DI container). Nothing else in the codebase knows which concrete class is used. In tests, replace the provider function with one that injects an in-memory implementation — no other code changes needed.

---

## Aggregate Design Heuristics

Use these rules when deciding aggregate boundaries:

| Question | Guidance |
|---|---|
| Should these two objects always be consistent together? | Put them in the same aggregate. |
| Can they be eventually consistent? | Put them in separate aggregates; use domain events to sync. |
| Is one object the "owner" that controls access? | That object is the aggregate root. |
| Does removing the root make the child meaningless? | Child belongs inside the aggregate. |
| Are you loading thousands of objects to change one? | Aggregate is too large — split it. |

**Practical example — Order vs. Customer:** A `Customer` aggregate should hold only `order_id` references, not full `Order` objects. Loading full `Order` objects every time `Customer` is fetched makes the aggregate too large. `Order` is its own aggregate root; `Customer` holds a lightweight `customer_id` back-reference.

---

## Domain Events — Publishing and Handling

Domain events decouple aggregates that need to react to each other's state changes. When an `Order` is submitted, it appends an `OrderSubmittedEvent` to an internal list. The use case extracts those events via `pop_events()` and writes them to an outbox table in the same database transaction as the aggregate state (transactional outbox pattern). A separate relay process publishes the events to the message broker, guaranteeing at-least-once delivery.

---

## Detecting and Breaking Dependency Cycles

Common symptoms and their structural fixes:

```
Symptom: use_cases/create_order.py imports from adapters/email_sender.py
Fix:     Create domain/interfaces/notification_service.py (abstract port).
         use_cases imports the port. adapters implements it.
         DI container wires them together.

Symptom: domain/entities/user.py imports from infrastructure/config.py
Fix:     Pass config values as constructor arguments or environment at
         the infrastructure boundary. Domain entities must not read config.

Symptom: Two aggregates import each other
Fix:     Introduce a domain event. Aggregate A emits OrderPlaced.
         Aggregate B's use case subscribes and reacts. They never import
         each other.

Symptom: Repository imports a use case to "do extra work" after saving
Fix:     Extract the extra work into a separate domain service or use case.
         Repositories persist state only; they do not orchestrate behaviour.
```

To verify dependency direction, draw a module graph and check that no arrow from `domain/` or `use_cases/` points outward toward `adapters/` or `infrastructure/`.
